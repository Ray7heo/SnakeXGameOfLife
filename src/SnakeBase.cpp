#include "../include/SnakeBase.h"

#include <iostream>


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
    if (body.empty() || (direction.x == 0 && direction.y == 0)) return;
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

        DrawCircle(body.front().x * config.tileSize + config.tileSize / 2,
                   body.front().y * config.tileSize + config.tileSize / 2, config.tileSize / 2,
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
            DrawCircle(body.back().x * config.tileSize + config.tileSize / 2,
                       body.back().y * config.tileSize + config.tileSize / 2, config.tileSize / 1.5,
                       tailColor);
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

rapidjson::Value SnakeBase::toJson(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value value(rapidjson::kObjectType);

    // 序列化 body
    rapidjson::Value bodyArray(rapidjson::kArrayType);
    for (const auto& vec : body)
    {
        rapidjson::Value vecValue(rapidjson::kObjectType);
        vecValue.AddMember("x", vec.x, allocator);
        vecValue.AddMember("y", vec.y, allocator);
        bodyArray.PushBack(vecValue, allocator);
    }
    value.AddMember("body", bodyArray, allocator);

    // 序列化 headColor
    value.AddMember("headColor", serializeColor(headColor, allocator), allocator);

    // 序列化 tailColor
    value.AddMember("tailColor", serializeColor(tailColor, allocator), allocator);

    // 序列化 isDead
    value.AddMember("isDead", isDead, allocator);

    // 序列化 direction
    rapidjson::Value directionValue(rapidjson::kObjectType);
    directionValue.AddMember("x", direction.x, allocator);
    directionValue.AddMember("y", direction.y, allocator);
    value.AddMember("direction", directionValue, allocator);

    return value;
}

SnakeBase SnakeBase::fromJson(const rapidjson::Value& json)
{
    SnakeBase snake;

    // 反序列化 body
    const rapidjson::Value& bodyArray = json["body"];
    for (rapidjson::SizeType i = 0; i < bodyArray.Size(); i++)
    {
        const rapidjson::Value& vecValue = bodyArray[i];
        const float x = vecValue["x"].GetFloat();
        const float y = vecValue["y"].GetFloat();
        snake.body.push_back(Vector2{x, y});
    }

    // 反序列化 headColor
    const rapidjson::Value& headColorValue = json["headColor"];

    snake.headColor = deserializeColor(headColorValue);

    // 反序列化 tailColor
    const rapidjson::Value& tailColorValue = json["tailColor"];
    snake.tailColor = deserializeColor(tailColorValue);

    // 反序列化 isDead
    snake.isDead = json["isDead"].GetBool();

    // 反序列化 direction
    const rapidjson::Value& directionValue = json["direction"];
    const float dirX = directionValue["x"].GetFloat();
    const float dirY = directionValue["y"].GetFloat();
    snake.direction = Vector2{dirX, dirY};

    return snake;
}

rapidjson::Value SnakeBase::serializeColor(const Color& color, rapidjson::Document::AllocatorType& allocator)
{
    rapidjson::Value colorValue(rapidjson::kObjectType);
    colorValue.AddMember("r", color.r, allocator);
    colorValue.AddMember("g", color.g, allocator);
    colorValue.AddMember("b", color.b, allocator);
    colorValue.AddMember("a", color.a, allocator);
    return colorValue;
}

Color SnakeBase::deserializeColor(const rapidjson::Value& json)
{
    int r = json["r"].GetInt();
    int g = json["g"].GetInt();
    int b = json["b"].GetInt();
    int a = json["a"].GetInt();

    return Color{
        static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b),
        static_cast<unsigned char>(a)
    };
}
