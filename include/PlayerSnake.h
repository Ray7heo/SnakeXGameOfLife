#pragma once
#include "Config.h"
#include "SnakeBase.h"
#include "../lib/RayLib/raylib.h"

class PlayerSnake final : public SnakeBase
{
public:
    PlayerSnake(const Color& headColor, const Color& tailColor, const Config& config, const Vector2& startPosition);

    void move() override;
    void autoMove(Vector2& foodPosition) override;
private:
    bool useArrowInput;
};
