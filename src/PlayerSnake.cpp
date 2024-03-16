#include "../include/PlayerSnake.h"


PlayerSnake::PlayerSnake(const Color& snakeColor, const Config& config): SnakeBase(snakeColor, config)
{}

void PlayerSnake::move()
{
    const auto isOnlyHead = body.size() < 2;
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
    {
        if (direction.y != 1 || isOnlyHead)
            direction = {0, -1};
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
    {
        if (direction.y != -1 || isOnlyHead)
            direction = {0, 1};
    }
    else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
    {
        if (direction.x != 1 || isOnlyHead)
            direction = {-1, 0};
    }
    else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
    {
        if (direction.x != -1 || isOnlyHead)
            direction = {1, 0};
    }
    SnakeBase::move();
}

void PlayerSnake::autoMove(Vector2& foodPosition)
{
}


