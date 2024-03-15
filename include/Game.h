#pragma once
#include <iostream>

#include "Button.h"
#include "GameState.h"
#include "PlayerSnake.h"
#include "../lib/RayLib/raylib.h"

class Game
{
public:
    Config config;
    std::unique_ptr<SnakeBase> snake;
    Vector2 food;
    Vector2 direction;
    GameState gameState;
    Button startButton;
    Button pauseButton;
    Button restartButton;
    Button menuButton;
    int score;

    Game(const Config& config, SnakeBase& snake);

    Game();
    void update();
    void draw();

private:
    void spawn_food();
};
