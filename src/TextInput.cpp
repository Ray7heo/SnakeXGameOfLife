#include "../include/TextInput.h"

#include <iostream>

TextInput::TextInput(const Rectangle rect, std::string defaultText): rect(rect), text(std::move(defaultText))
{}

void TextInput::update()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            active = true;
        } else {
            active = false;
        }
    }

    if (active) {
        const int key = GetKeyPressed();
        if (key > 0 && key != KEY_BACKSPACE) {
            std::cout << key << "\n";
            text += static_cast<char>(key);
        } else if (key == KEY_BACKSPACE && text.length() > 0) {
            text.pop_back();
        }
    }
}

void TextInput::draw() const
{
    DrawRectangleLinesEx(rect, 1, BLACK);
    DrawText(text.c_str(), rect.x + 5, rect.y + 5, 20, BLACK);
}
