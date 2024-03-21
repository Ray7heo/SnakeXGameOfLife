#pragma once
#include <string>
#include <raylib.h>

class Button
{
public:
    Rectangle bounds;
    Color color;
    Color textColor;
    std::string text;

    Button(const Rectangle& bounds, std::string text);

    Button(const Rectangle& bounds, const Color& color, const Color& textColor, std::string text);
    bool isClicked() const;

    void draw() const;
};
