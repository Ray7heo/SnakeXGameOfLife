#include "../include/ClickableRectangle.h"

ClickableRectangle::ClickableRectangle(const Rectangle& bounds, const Color& color): bounds(bounds),
    color(color)
{
}

bool ClickableRectangle::isClicked(const Vector2 mousePos) const
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, bounds);
}

ClickableRectangle ClickableRectangle::draw() const
{
    DrawRectangleRec(bounds, color);
    return *this;
}
