#include "../include/Button.h"

Button::Button(const Rectangle& bounds, const Color& color, const Color& text_color, std::string text): bounds(bounds),
    color(color),
    textColor(text_color),
    text(std::move(text))
{
}

bool Button::isClicked(Vector2 mousePos) const
{
    return CheckCollisionPointRec(mousePos, bounds);
}


void Button::draw() const
{
    DrawRectangleRec(bounds, color);
    DrawText(text.c_str(), bounds.x + bounds.width / 2 - MeasureText(text.c_str(), 20) / 2, bounds.y + bounds.height / 2 - 10, 20, textColor);
}
