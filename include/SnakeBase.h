#pragma once
#include <vector>

#include "Config.h"
#include "../lib/RayLib/raylib.h"

class SnakeBase
{
public:
    virtual ~SnakeBase() = default;
    Vector2 position;
    Color color;
    std::vector<Vector2> body;
    SnakeBase(const Color& color, const Config& config);
    virtual void move(Vector2 dir);
    void draw() const;
    void grow();
    Rectangle getCollisionRec() const;
private:
    Config config; 
};
