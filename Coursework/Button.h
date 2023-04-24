#pragma once
#include <functional>
#include "TextBox.h"

class Button
{
public:
    using Action = std::function<void()>; // Define an action type

    Button(const Rectangle& rect, const std::string& text, Action action, 
        int fontSize = 20, Color textColor = BLACK, Color boxColor = LIGHTGRAY, Color hoverColor = Color{ 130,130,130, 50 });

    void Draw() const;
    void Update();
    bool IsClicked() const;
    bool IsHovered() const;

private:
    Rectangle rectBox;
    TextBox textBox;
    Color boxColor;
    Color hoverColor;
    Action action;
};
