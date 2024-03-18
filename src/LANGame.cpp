#include "../include/LANGame.h"

LANGame::LANGame()
= default;

LANGame::LANGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake), textInput({0, 0, 200, 30}, "")
{
}

void LANGame::update()
{
    if (gameState == GameState::Playing)
    {
        snake->move();
    }
    GameBase::update();
}

void LANGame::restart()
{
    GameBase::restart();
    snake = std::make_unique<
        PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}

void LANGame::draw()
{
    if (gameState == GameState::Start)
    {
        textInput.draw();
        textInput.update();
    }
    GameBase::draw();
}
