#include "../include/Snake.h"


Snake::Snake(const Color& color, const Config& config):
    color(color),
    config(config)
{
    auto v = Vector2();
    v.x = config.screenWidth / 2;
    v.y = config.screenHeight / 2;
    position = v;
    body.push_back(position);
}



void Snake::Move(Vector2 dir) {
    position.x += dir.x * config.squareSize;
    position.y += dir.y * config.squareSize;

    // Move body
    for (int i = body.size() - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    body[0] = position;
}

void Snake::Draw() {
    DrawRectangle(position.x, position.y, config.squareSize, config.squareSize, color);

    for (size_t i = 1; i < body.size(); i++) {
        DrawRectangle(body[i].x, body[i].y, config.squareSize, config.squareSize, color);
    }
}

void Snake::Grow() {
    body.push_back(body[body.size() - 1]);
}

Rectangle Snake::GetCollisionRec()
{
    return { position.x, position.y, (float)config.squareSize, (float)config.squareSize };
}
