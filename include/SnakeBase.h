#pragma once
#include <vector>
#include <queue>
#include <unordered_map>

#include "Config.h"
#include "../lib/RayLib/raylib.h"

class SnakeBase
{
public:
    virtual ~SnakeBase() = default;
    std::vector<Vector2> body;
    Color headColor;
    Color tailColor;
    bool isDead;
    Vector2 direction;

    SnakeBase(const Color& headColor, const Color& tailColor, const Config& config, Vector2 startPosition);
    virtual void move();
    virtual void autoMove(Vector2& foodPosition);
    void draw() const;
    void grow();
    Rectangle getCollisionRec() const;

protected:
    Config config;
};
