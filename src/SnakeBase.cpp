#include "../include/SnakeBase.h"


SnakeBase::SnakeBase(const Color& color, const Config& config):
    color(color),
    config(config)
{
    auto v = Vector2();
    v.x = config.screenWidth / 2;
    v.y = config.screenHeight / 2;
    position = v;
    body.push_back(position);
}


void SnakeBase::move(Vector2& direction)
{
    position.x += direction.x * config.squareSize;
    position.y += direction.y * config.squareSize;

    // Move body
    for (int i = body.size() - 1; i > 0; i--)
    {
        body[i] = body[i - 1];
    }
    body[0] = position;


    // Check collision with walls
    if (position.x >= config.screenWidth || position.x < 0 || position.y >= config.screenHeight || position.y < 0)
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

void SnakeBase::draw() const
{
    // draw head
    DrawRectangle(position.x, position.y, config.squareSize, config.squareSize, BLUE);

    // draw body
    for (size_t i = 1; i < body.size() - 1; i++)
    {
        DrawRectangle(body[i].x, body[i].y, config.squareSize, config.squareSize, color);
    }

    if (body.size() > 1)
    {
        DrawRectangle(body[body.size() - 1].x, body[body.size() - 1].y, config.squareSize, config.squareSize,
                      BLACK);
    }
}

void SnakeBase::grow()
{
    body.push_back(body[body.size() - 1]);
}

Rectangle SnakeBase::getCollisionRec() const
{
    return {position.x, position.y, static_cast<float>(config.squareSize), static_cast<float>(config.squareSize)};
}
