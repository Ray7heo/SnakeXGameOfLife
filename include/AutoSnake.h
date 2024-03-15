#pragma once
#include "AStart.h"
#include "SnakeBase.h"

class AutoSnake final : public SnakeBase
{
public:
    AutoSnake(const Color& color, const Config& config);

    void move() override;

    void autoMove(Vector2& foodPosition) override;

private:
    AStar aStar;
};
