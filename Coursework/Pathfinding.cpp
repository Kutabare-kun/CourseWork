#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "Pathfinding.h"


struct Vector2Hash {
    std::size_t operator()(const Vector2& v) const {
        return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
    }
};


struct Vector2Equal {
    bool operator()(const Vector2& lhs, const Vector2& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};


float Pathfinding::Heuristic(const Vector2& a, const Vector2& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}


std::vector<Vector2> Pathfinding::FindPath(const Grid& grid, const Vector2& start, const Vector2& end) {
    using Node = std::pair<Vector2, float>;
    auto cmp = [](const Node& a, const Node& b) { return a.second > b.second; };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openList(cmp);
    std::unordered_set<Vector2, Vector2Hash, Vector2Equal> openListSet;
    std::unordered_set<Vector2, Vector2Hash, Vector2Equal> closedList;
    std::unordered_map<Vector2, Vector2, Vector2Hash, Vector2Equal> cameFrom;
    std::unordered_map<Vector2, float, Vector2Hash, Vector2Equal> gScore;

    openList.emplace(start, 0.0f);
    openListSet.insert(start);
    gScore[start] = 0.0f;

    while (!openList.empty()) {
        Vector2 currentNode = openList.top().first;
        openList.pop();
        openListSet.erase(currentNode);
        closedList.insert(currentNode);

        if (currentNode.x == end.x && currentNode.y == end.y) {
            std::vector<Vector2> path;
            while (currentNode.x != start.x || currentNode.y != start.y) {
                path.push_back(grid.GridToWorld(currentNode.x, currentNode.y));
                currentNode = cameFrom[currentNode];
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        std::vector<Vector2> neighbors = {
            Vector2{currentNode.x + 1, currentNode.y},
            Vector2{currentNode.x - 1, currentNode.y},
            Vector2{currentNode.x, currentNode.y + 1},
            Vector2{currentNode.x, currentNode.y - 1}
        };

        for (const Vector2& neighbor : neighbors) {
            if (closedList.find(neighbor) != closedList.end() || !grid.IsWalkable(neighbor.x, neighbor.y)) {
                continue;
            }

            float tentative_gScore = gScore[currentNode] + 1;

            if (openListSet.find(neighbor) == openListSet.end() || tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = currentNode;
                gScore[neighbor] = tentative_gScore;
                float fScore = tentative_gScore + Heuristic(neighbor, end);

                if (openListSet.find(neighbor) == openListSet.end()) {
                    openList.emplace(neighbor, fScore);
                    openListSet.insert(neighbor);
                }
            }
        }
    }
    return {};
}
