#pragma once
#include <unordered_map>
#include "SnakeBase.h"

class AutoSnake final : public SnakeBase
{
public:
    AutoSnake(const Color& headColor, const Color& tailColor, const Config& config, Vector2 startPosition);

    void move() override;

    void autoMove(Vector2& foodPosition) override;

private:
    std::vector<Vector2> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::vector<Vector2> findPath(const Vector2& start, const Vector2& end) const;
};
