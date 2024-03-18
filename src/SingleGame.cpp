#include "../include/SingleGame.h"

SingleGame::SingleGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake)
{
}

SingleGame::SingleGame()
{
    snake = std::make_unique<PlayerSnake>(RED,BLUE, config, Vector2{1, static_cast<float>(config.gridHeight / 2)});
}

void SingleGame::update()
{
    if (gameState == GameState::Playing)
    {
        snake->move();
    }
    GameBase::update();
}

void SingleGame::draw()
{
    GameBase::draw();
}

void SingleGame::restart()
{
    GameBase::restart();
    snake = std::make_unique<
        PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}
