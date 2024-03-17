#include "../include/PVPLocalGame.h"

PVPLocalGame::PVPLocalGame(): rightScore(0)
{
    rightSnake = std::make_unique<PlayerSnake>(BLUE,BLACK, config, Vector2{
                                                   static_cast<float>(config.gridWidth - 1),
                                                   static_cast<float>(config.gridHeight / 2)
                                               }, true);


    snake = std::make_unique<
        PlayerSnake>(RED,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight / 2)}, false);
}

PVPLocalGame::PVPLocalGame(const GameConfig& config, SnakeBase& leftSnake, SnakeBase& rightSnake):
    GameBase(config, leftSnake), rightSnake(&rightSnake),
    rightScore(0)
{
}


void PVPLocalGame::update()
{
    if (gameState == GameState::Playing)
    {
        rightSnake->move();
        snake->move();

        if (CheckCollisionRecs(rightSnake->getCollisionRec(), {
                                   food.x * config.tileSize, food.y * config.tileSize,
                                   static_cast<float>(config.tileSize),
                                   static_cast<float>(config.tileSize)
                               }))
        {
            rightScore++;
            rightSnake->grow();
            spawnFood();
        }
        if (rightSnake->isDead)
        {
            gameState = GameState::GameOver;
        }
    }
    GameBase::update();
}

void PVPLocalGame::draw()
{
    if (gameState == GameState::Playing)
    {
        rightSnake->draw();
        // draw score
        char scoreText[30];
        sprintf_s(scoreText, "RightPlayer Score: %d", rightScore);
        DrawText(scoreText, 10, 30, 20, BLACK);
    }
    else if (gameState == GameState::Paused)
    {
        char scoreText[30];
        sprintf_s(scoreText, "RightPlayer Score: %d", rightScore);
        DrawText(scoreText, 10, 30, 20, BLACK);
    }
    GameBase::draw();
}

void PVPLocalGame::restart()
{
    GameBase::restart();
    rightScore = 0;
    rightSnake = std::make_unique<PlayerSnake>(BLUE,BLACK, config, Vector2{
                                                   static_cast<float>(config.gridWidth - 1),
                                                   static_cast<float>(config.gridHeight / 2)
                                               }, true);
    snake = std::make_unique<
        PlayerSnake>(RED,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight / 2)}, false);
}
