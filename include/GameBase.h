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
    class FindCellByPosition
    {
    public:
        explicit FindCellByPosition(Vector2 targetPos);

        bool operator()(const Cell& cell) const;

    private:
        Vector2 targetPos;
    };

    class FindCellByType
    {
    public:
        explicit FindCellByType(const CellType& type);

        bool operator()(const Cell& cell) const;

    private:
        CellType type;
    };

    std::unique_ptr<SnakeBase> snake;
    Button startButton;
    Button pauseButton;
    Button restartButton;
    Button menuButton;
    std::vector<Cell> cells;

    void spawnFood();
    virtual void restart();


private:
    void randomCell();
    Vector2 randomCellPosition();
};
