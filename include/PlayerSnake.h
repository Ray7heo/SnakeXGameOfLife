#pragma once
#include "GameConfig.h"
#include "SnakeBase.h"
#include <raylib.h>

class PlayerSnake final : public SnakeBase
{
public:
    PlayerSnake(const Color& headColor, const Color& tailColor, const GameConfig& config, const Vector2& startPosition,
                bool useArrowInput = false);

    void move() override;
    void autoMove(Vector2& foodPosition) override;

private:
    bool useArrowInput;
};
