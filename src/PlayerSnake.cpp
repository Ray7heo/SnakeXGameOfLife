#include "../include/PlayerSnake.h"


PlayerSnake::PlayerSnake(const Color& headColor, const Color& tailColor, const Config& config,
                         const Vector2& startPosition,
                         const bool useArrowInput): SnakeBase(headColor, tailColor, config, startPosition),
                                                    useArrowInput(useArrowInput)
{
}

void PlayerSnake::move()
{
    const auto isOnlyHead = body.size() < 2;
    if (useArrowInput)
    {
        if (IsKeyPressed(KEY_UP))
        {
            if (direction.y != 1 || isOnlyHead)
                direction = {0, -1};
        }
        else if (IsKeyPressed(KEY_DOWN))
        {
            if (direction.y != -1 || isOnlyHead)
                direction = {0, 1};
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            if (direction.x != 1 || isOnlyHead)
                direction = {-1, 0};
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            if (direction.x != -1 || isOnlyHead)
                direction = {1, 0};
        }
    }
    else
    {
        if (IsKeyPressed(KEY_W))
        {
            if (direction.y != 1 || isOnlyHead)
                direction = {0, -1};
        }
        else if (IsKeyPressed(KEY_S))
        {
            if (direction.y != -1 || isOnlyHead)
                direction = {0, 1};
        }
        else if (IsKeyPressed(KEY_A))
        {
            if (direction.x != 1 || isOnlyHead)
                direction = {-1, 0};
        }
        else if (IsKeyPressed(KEY_D))
        {
            if (direction.x != -1 || isOnlyHead)
                direction = {1, 0};
        }
    }
    SnakeBase::move();
}

void PlayerSnake::autoMove(Vector2& foodPosition)
{
}
