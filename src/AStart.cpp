#include "../include/AStart.h"

AStar::AStar(int width, int height): width(width), height(height)
{}

bool AStar::isValid(Vector2 p) const
{
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

float AStar::manhattanDistance(Vector2 a, Vector2 b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<Vector2> AStar::findPath(Vector2 start, Vector2 end) const
{
    std::queue<Vector2> openList;
    std::vector<std::vector<bool>> closedList(height, std::vector<bool>(width, false));
    std::vector<std::vector<Vector2>> cameFrom(height, std::vector<Vector2>(width));

    openList.push(start);

    while (!openList.empty()) {
        Vector2 current = openList.front();
        openList.pop();

        if (current.x == end.x && current.y == end.y) {
            std::vector<Vector2> path;
            while (current.x != start.x || current.y != start.y) {
                path.push_back(current);
                current = cameFrom[current.y][current.x];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedList[current.y][current.x] = true;

        for (auto& dir : directions) {
            Vector2 nextPos = {current.x + dir.x, current.y + dir.y};
            if (isValid(nextPos) && !closedList[nextPos.y][nextPos.x]) {
                openList.push(nextPos);
                cameFrom[nextPos.y][nextPos.x] = current;
            }
        }
    }

    return {}; // No path found
}
