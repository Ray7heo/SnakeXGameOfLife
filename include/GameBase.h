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
    bool isMapReady = false;
    std::unique_ptr<SnakeBase> snake;
    Button startButton;
    Button pauseButton;
    Button restartButton;
    Button menuButton;
    std::vector<std::vector<std::shared_ptr<Cell>>> cells;

    virtual void restart();

    virtual int countLiveNeighbors(int x, int y) const;

private:
    std::thread updateThread;
    void randomCell();
    void updateCell() const;
};
