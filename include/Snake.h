#pragma once
#include <vector>

#include "Config.h"
#include "../lib/RayLib/raylib.h"

class Snake {
public:
    Vector2 position;
    Color color;
    std::vector<Vector2> body;
    Snake(const Color& color, const Config& config);
    void Move(Vector2 dir);
    void Draw();
    void Grow();
    Rectangle GetCollisionRec();
private:
    Config config;
};
