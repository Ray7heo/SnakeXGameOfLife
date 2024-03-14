#pragma once
#include <string>
#include "../lib/RayLib/raylib.h"

class Button
{
public:
    Rectangle bounds;
    Color color;
    Color textColor;
    std::string text;

    Button(const Rectangle& bounds, const Color& color, const Color& text_color, std::string text);
    bool isClicked(Vector2 mousePos) const;

    void draw() const;
};
