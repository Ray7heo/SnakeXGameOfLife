#pragma once
#include <queue>
#include <vector>
#include "../lib/RayLib/raylib.h"

class AStar {
private:
    std::vector<Vector2> directions{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int width, height;
public:
    AStar(int width, int height);

    bool isValid(Vector2 p) const;

    static float manhattanDistance(Vector2 a, Vector2 b);

    std::vector<Vector2> findPath(Vector2 start, Vector2 end) const;
};
