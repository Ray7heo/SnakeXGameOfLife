#pragma once

#include "raylib.h"
#include <string>

class TextInput
{
public:
    TextInput() = default;
    explicit TextInput(Rectangle rect, std::string defaultText = "");

    void update();

    void draw() const;

    std::string text;
private:
    Rectangle rect;
    bool active = false;
};
