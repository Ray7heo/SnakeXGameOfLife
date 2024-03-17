#pragma once
#include <iostream>
#include <raylib.h>

#include "Button.h"
#include "PlayerSnake.h"

enum class GameState
{
    Start,
    Playing,
    Paused,
    GameOver,
    Menu
};

class GameBase
{
public:
    virtual ~GameBase() = default;
    GameConfig config;
    GameState gameState;
    int score;

    GameBase();
    
    GameBase(const GameConfig& config, SnakeBase& snake);

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
