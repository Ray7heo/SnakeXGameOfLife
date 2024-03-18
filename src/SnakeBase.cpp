#include "../include/SnakeBase.h"


SnakeBase::SnakeBase(const Color& headColor, const Color& tailColor, const GameConfig& config,
                     const Vector2 startPosition):
    headColor(headColor), tailColor(tailColor), isDead(false), direction(Vector2{0, 0}),
    config(config)
{
    body.push_back(startPosition);
}

void SnakeBase::move()
{
    // Move body
    const auto nexPos = Vector2{body.front().x + direction.x, body.front().y + direction.y};
    body.insert(body.begin(), nexPos);
    body.pop_back();

    // Check collision with walls
    if (body.front().x >= static_cast<float>(config.gridWidth) || body.front().x < 0 || body.front().y >= static_cast<
        float>(config.gridHeight) || body.front().y < 0)
    {
        isDead = true;
    }

    // Check self-collision
    if (body.size() >= 2)
    {
        for (size_t i = 1; i < body.size(); i++)
        {
            if (body.front().x == body[i].x && body.front().y == body[i].y)
            {
                isDead = true;
                break;
            }
        }
    }
}


void SnakeBase::autoMove(Vector2& foodPosition)
{
}

void SnakeBase::draw() const
{
    if (!isDead)
    {
        // draw head
        DrawRectangle(body.front().x * config.tileSize, body.front().y * config.tileSize, config.tileSize,
                      config.tileSize,
                      headColor);

        // draw body
        for (size_t i = 1; i < body.size() - 1; i++)
        {
            DrawRectangle(body[i].x * config.tileSize, body[i].y * config.tileSize, config.tileSize, config.tileSize,
                          GREEN);
        }

        // draw tail
        if (body.size() > 1)
        {
            DrawRectangle(body.back().x * config.tileSize, body.back().y * config.tileSize,
                          config.tileSize, config.tileSize, tailColor);
        }
    }
}

void SnakeBase::grow()
{
    body.push_back(body.back());
}

void SnakeBase::shrink()
{
    if (body.size() <= 1)
    {
        isDead = true;
    }
    else
    {
        body.pop_back();
    }
}

Rectangle SnakeBase::getCollisionRec() const
{
    return {
        body.front().x * static_cast<float>(config.tileSize), body.front().y * static_cast<float>(config.tileSize),
        static_cast<float>(config.tileSize),
        static_cast<float>(config.tileSize)
    };
}
