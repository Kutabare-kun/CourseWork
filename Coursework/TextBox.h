#pragma once
#include "raylib.h"
#include <string>

class TextBox
{
public:
    TextBox(const Rectangle& rect, const std::string& text, int fontSize = 20, Color textColor = BLACK);

    void Draw() const;
    static size_t CalculateMaxChars(const float& width, const int& font_size = 20);
    static float CalculateWidthText(const char* text, const int& font_size = 20);

    const Vector2& GetPos() const { return pos; }
    const std::string& GetText() const { return text; }
    const int& GetFontSize() const { return fontSize; }

    void SetText(const std::string& text) { this->text = text; }
    void SetPos(const Vector2& position) { pos = position; }

private:

    Vector2 pos;
    std::string text;
    int fontSize;
    Color textColor;
};
	