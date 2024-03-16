#pragma once
#include <iostream>

#include "Button.h"
#include "GameState.h"
#include "PlayerSnake.h"
#include "../lib/RayLib/raylib.h"

class GameBase
{
public:
    virtual ~GameBase() = default;
    Config config;
    GameState gameState;
    int score;

    GameBase();
    
    GameBase(const Config& config, SnakeBase& snake);

    virtual void update();
    virtual void draw();

protected:
    std::unique_ptr<SnakeBase> snake;
    Vector2 food;
    Button startButton;
    Button pauseButton;
    Button restartButton;
    Button menuButton;

    void spawnFood();
    virtual void restart();
};
