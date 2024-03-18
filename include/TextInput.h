#pragma once

#include "raylib.h"
#include <string>

class TextInput
{
public:
    TextInput() = default;
    explicit TextInput(Rectangle bounds, std::string defaultText = "");

    void update();

    void draw() const;

    std::string text;
    Rectangle bounds;

private:
    bool active = false;
};
