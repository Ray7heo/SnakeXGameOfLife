#pragma once
#include <raylib.h> 

class ClickableRectangle
{
public:
    Rectangle bounds;
    Color color;

    ClickableRectangle(const Rectangle& bounds, const Color& color);
    bool isClicked(Vector2 mousePos) const;
    ClickableRectangle draw() const;
};
