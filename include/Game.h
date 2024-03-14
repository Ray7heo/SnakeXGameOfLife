#pragma once
#include "GameState.h"
#include "Snake.h"

class Game
{
public:
    Snake snake;
    Vector2 food;
    Vector2 direction;
    bool gameOver;
    GameState gameState;

    Game(const Vector2& food, const Config& config);

    Game();
    void Update();
    void Draw();
    void SpawnFood();

private:
    Config config;
};
