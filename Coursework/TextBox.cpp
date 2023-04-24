#include "TextBox.h"
#include "raylib.h"


TextBox::TextBox(const Rectangle& rect, const std::string& text, int fontSize, Color textColor)
    : text(text), fontSize(fontSize), textColor(textColor)
{
    // Measure the text size using the font size
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(fontSize), 0);

    // Calculate the center position of the text within the rectangle
    pos.x = rect.x + (rect.width / 2) - (textSize.x / 2);
    pos.y = rect.y + (rect.height / 2) - (textSize.y / 2);
}


void TextBox::Draw() const
{
    DrawText(text.c_str(), pos.x, pos.y, fontSize, textColor);
}


size_t TextBox::CalculateMaxChars(const float& width, const int& font_size)
{
    return width / MeasureText("A", font_size);
}

float TextBox::CalculateWidthText(const char* text, const int& font_size)
{
    return MeasureText(text, font_size);
}
