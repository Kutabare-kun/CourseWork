#pragma once

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void onActivate() = 0;
    virtual void onDeactivate() = 0;
};