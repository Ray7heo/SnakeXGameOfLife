#pragma once
#include "Config.h"
#include "SnakeBase.h"
#include "../lib/RayLib/raylib.h"

class PlayerSnake final : public SnakeBase
{
public:
    PlayerSnake(const Color& snakeColor, const Config& config);
    void move(Vector2& direction) override;
};
