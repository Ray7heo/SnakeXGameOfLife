#pragma once
#include <vector>

#include "Config.h"
#include "../lib/RayLib/raylib.h"

class SnakeBase
{
public:
    virtual ~SnakeBase() = default;
    Vector2 position;
    std::vector<Vector2> body;
    Color color;
    bool isDead;
    Vector2 direction;
    
    SnakeBase(const Color& color, const Config& config);
    virtual void move();
    virtual void autoMove(Vector2& foodPosition);
    void draw() const;
    void grow();
    Rectangle getCollisionRec() const;
    

protected:
    Config config;
};
