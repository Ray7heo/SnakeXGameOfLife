#pragma once
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <raylib.h>

#include "Button.h"
#include "Cell.h"
#include "SnakeBase.h"
#include "../include/PlayerSnake.h"
#include "../include/AutoSnake.h"


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
    Button startButton;
    Button pauseButton;
    Button restartButton;
    Button menuButton;
    std::vector<std::vector<std::shared_ptr<Cell>>> cells;

    virtual void restart();

private:
    void randomCell();
    // static CellType randomType(const std::vector<WeightedCell>& weightedCells);
};
