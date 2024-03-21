#include "../include/Button.h"

Button::Button(const Rectangle& bounds, std::string text): bounds(bounds), color(GREEN), textColor(BLACK),
                                                           text(std::move(text))
{
}

Button::Button(const Rectangle& bounds, const Color& color, const Color& textColor, std::string text): bounds(bounds),
    color(color),
    textColor(textColor),
    text(std::move(text))
{
}

bool Button::isClicked() const
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), bounds);
}


void Button::draw() const
{
    DrawRectangleRec(bounds, color);
    DrawText(text.c_str(), bounds.x + bounds.width / 2 - MeasureText(text.c_str(), 20) / 2,
             bounds.y + bounds.height / 2 - 10, 20, textColor);
}
