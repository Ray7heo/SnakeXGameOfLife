#include "../include/SnakeBase.h"


SnakeBase::SnakeBase(const Color& color, const Config& config):
    color(color), isDead(false), direction(Vector2{1, 0}),
    config(config)
{
    position = Vector2{
        static_cast<float>(config.gridWidth - config.tileSize), static_cast<float>(config.gridHeight) / 2
    };
    body.push_back(position);
}

void SnakeBase::move()
{
    position.x += this->direction.x * config.tileSize;
    position.y += this->direction.y * config.tileSize;

    // Move body
    for (int i = body.size() - 1; i > 0; i--)
    {
        body[i] = body[i - 1];
    }

    // Check collision with walls
    if (position.x >= config.gridWidth * config.tileSize || position.x < 0 || position.y >= config.gridHeight * config.
        tileSize || position.y < 0)
    {
        isDead = true;
    }

    // Check self-collision
    if (body.size() >= 2)
    {
        for (size_t i = 1; i < body.size(); i++)
        {
            if (position.x == body[i].x && position.y == body[i].y)
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
    // draw head
    DrawRectangle(body.front().x * config.tileSize, body.front().y * config.tileSize, config.tileSize, config.tileSize,
                  BLUE);

    // draw body
    for (size_t i = 1; i < body.size() - 1; i++)
    {
        DrawRectangle(body[i].x * config.tileSize, body[i].y * config.tileSize, config.tileSize, config.tileSize,
                      color);
    }

    // draw tail
    if (body.size() > 1)
    {
        DrawRectangle(body.back().x * config.tileSize, body.back().y * config.tileSize,
                      config.tileSize, config.tileSize, BLACK);
    }
}

void SnakeBase::grow()
{
    body.push_back(body.back());
}

Rectangle SnakeBase::getCollisionRec() const
{
    return {
        body.front().x * static_cast<float>(config.tileSize), body.front().y * static_cast<float>(config.tileSize), static_cast<float>(config.tileSize),
        static_cast<float>(config.tileSize)
    };
}