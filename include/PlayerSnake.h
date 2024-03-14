#pragma once
#include "Config.h"
#include "SnakeBase.h"
#include "../lib/RayLib/raylib.h"

class PlayerSnake : public SnakeBase {
public:
    PlayerSnake(const Color& snakeColor, const Config& config);
    void move(Vector2 dir) override;
};
