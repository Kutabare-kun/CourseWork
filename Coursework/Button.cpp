#include "Button.h"


Button::Button(const Rectangle& rect, const std::string& text, Action action, int fontSize, Color textColor, Color boxColor, Color hoverColor)
    : rectBox(rect), textBox(rect, text, fontSize, textColor), boxColor(boxColor), hoverColor(hoverColor), action(action)
{}


void Button::Draw() const
{
    DrawRectangleRec(rectBox, boxColor);
    if (IsHovered())
        DrawRectangleRec(rectBox, hoverColor);
    textBox.Draw();
}


void Button::Update()
{
    if (IsClicked())
        action();
}


bool Button::IsClicked() const
{
    return CheckCollisionPointRec(GetMousePosition(), rectBox)
		&& IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}


bool Button::IsHovered() const
{
    return CheckCollisionPointRec(GetMousePosition(), rectBox);
}
