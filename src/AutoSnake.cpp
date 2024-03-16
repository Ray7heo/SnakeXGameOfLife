#include "../include/AutoSnake.h"

AutoSnake::AutoSnake(const Color& headColor, const Color& tailColor, const Config& config,
                     const Vector2 startPosition): SnakeBase(headColor, tailColor, config, startPosition)
{
}

void AutoSnake::move()
{
}

void AutoSnake::autoMove(Vector2& foodPosition)
{
    const auto path = findPath(body[0], foodPosition);

    // Move the snake
    if (!path.empty() && path.size() >= 2)
    {
        body.insert(body.begin(), path[1]);
        body.pop_back();
    }
    else
    {
        // random move
        int direction = GetRandomValue(0, 3);
        Vector2 nextPos = {body.front().x + directions[direction].x, body.front().y + directions[direction].y};
        body.insert(body.begin(), nextPos);
        body.pop_back();
    }
}

struct Node
{
    Vector2 position;
    Node* parent;
    int gCost;
    int hCost;
    int fCost() const { return gCost + hCost; }
};

bool operator==(const Vector2& a, const Vector2& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Node& a, const Node& b)
{
    return a.position.x == b.position.x && a.position.y == b.position.y;
}

template <>
struct std::hash<Vector2>
{
    size_t operator()(const Vector2& vec) const noexcept
    {
        return hash<int>()(vec.x) ^ hash<int>()(vec.y);
    }
};

std::vector<Vector2> AutoSnake::findPath(const Vector2& start, const Vector2& end) const
{
    // Implement A* algorithm to find path from start to end
    std::vector<Vector2> result;
    std::vector<Node> openList;
    std::unordered_map<Vector2, Node> nodeMap;

    int hCost = static_cast<int>(abs(start.x - end.x) + abs(start.y - end.y)); // 将浮点数强制转换为整数

    Node startNode = {start, nullptr, 0, hCost};
    openList.push_back(startNode);
    nodeMap[start] = startNode;

    while (!openList.empty())
    {
        // Find node with lowest fCost in openList
        auto currentNode = std::min_element(openList.begin(), openList.end(), [](const Node& a, const Node& b)
        {
            return a.fCost() < b.fCost();
        });

        // Move current node from open list to closed list
        Node current = *currentNode;
        openList.erase(currentNode);

        // Check if current node is end
        if (current.position.x == end.x && current.position.y == end.y)
        {
            // Reconstruct path
            result.push_back(current.position);
            while (current.parent != nullptr)
            {
                current = *current.parent;
                result.push_back(current.position);
            }
            std::reverse(result.begin(), result.end());
            break;
        }

        // Generate children for current node
        for (const auto& dir : directions)
        {
            Vector2 nextPos = {current.position.x + dir.x, current.position.y + dir.y};
            // Check if next position is within bounds and not part of snake
            if (nextPos.x >= 0 && nextPos.x < config.gridWidth && nextPos.y >= 0 && nextPos.y < config.gridHeight)
            {
                bool validMove = true;
                for (const auto& segment : body)
                {
                    if (segment.x == nextPos.x && segment.y == nextPos.y)
                    {
                        validMove = false;
                        break;
                    }
                }
                if (validMove)
                {
                    int gCost = current.gCost + 1;
                    int hCost = static_cast<int>(abs(nextPos.x - end.x) + abs(nextPos.y - end.y)); // 将浮点数强制转换为整数
                    Node newNode = {nextPos, &nodeMap[current.position], gCost, hCost};
                    auto it = nodeMap.find(nextPos);
                    if (it == nodeMap.end() || gCost < it->second.gCost)
                    {
                        nodeMap[nextPos] = newNode;
                        openList.push_back(newNode);
                    }
                }
            }
        }
    }

    return result;
}
