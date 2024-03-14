#include "../include/PlayerSnake.h"


PlayerSnake::PlayerSnake(const Color& snakeColor, const Config& config): SnakeBase(snakeColor, config)
{}

void PlayerSnake::move(Vector2 dir)
{
    SnakeBase::move(dir);
}
