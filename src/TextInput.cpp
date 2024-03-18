#include "../include/TextInput.h"

#include <iostream>

TextInput::TextInput(const Rectangle bounds, std::string defaultText): bounds(bounds), text(std::move(defaultText))
{}

void TextInput::update()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
            active = true;
        } else {
            active = false;
        }
    }

    if (active) {
        const int key = GetKeyPressed();
        if (key > 0 && key != KEY_BACKSPACE && key != KEY_LEFT_SHIFT) {
            text += static_cast<char>(key);
        } else if (key == KEY_BACKSPACE && text.length() > 0) {
            text.pop_back();
        }
    }
}

void TextInput::draw() const
{
    DrawRectangleLinesEx(bounds, 1, BLACK);
    DrawText(text.c_str(), bounds.x + 5, bounds.y + 5, 20, BLACK);
}
