#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include "GameConfig.h"
#include <raylib.h>
#include <string>

#include "../lib/rapidjson/document.h"
#include "../lib/rapidjson/writer.h"
#include "../lib/rapidjson/stringbuffer.h"

class SnakeBase
{
public:
    virtual ~SnakeBase() = default;
    std::vector<Vector2> body;
    Color headColor;
    Color tailColor;
    bool isDead;
    Vector2 direction;

    SnakeBase(const Color& headColor, const Color& tailColor, const GameConfig& config, Vector2 startPosition);
    SnakeBase() = default;

    virtual void move();
    virtual void autoMove(Vector2& foodPosition);
    void draw() const;
    void grow();
    void shrink();
    Rectangle getCollisionRec() const;


    rapidjson::Value toJson(rapidjson::Document::AllocatorType& allocator) const;

    static SnakeBase fromJson(const rapidjson::Value& json);

    GameConfig config;

private:
    static rapidjson::Value serializeColor(const Color& color, rapidjson::Document::AllocatorType& allocator);

    static Color deserializeColor(const rapidjson::Value& json);
};
