#include "../include/PVEGame.h"

#include "../include/AutoSnake.h"


PVEGame::PVEGame(): autoScore(0)
{
    autoSnake = std::make_unique<AutoSnake>(BLUE,BLACK, config, Vector2{
                                                static_cast<float>(config.gridWidth),
                                                static_cast<float>(config.gridHeight / 2)
                                            });

    snake = std::make_unique<
        PlayerSnake>(RED,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}

PVEGame::PVEGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake), autoScore(0)
{
    autoSnake = std::make_unique<AutoSnake>(BLUE,BLACK, config, Vector2{
                                              static_cast<float>(config.gridWidth),
                                              static_cast<float>(config.gridHeight / 2)
                                          });
}

void PVEGame::update()
{
    if (gameState == GameState::Playing)
    {
        autoSnake->autoMove(food);
        snake->move();

        if (CheckCollisionRecs(autoSnake->getCollisionRec(), {
                                   food.x * config.tileSize, food.y * config.tileSize,
                                   static_cast<float>(config.tileSize),
                                   static_cast<float>(config.tileSize)
                               }))
        {
            autoScore++;
            autoSnake->grow();
            spawnFood();
        }
        if (autoSnake->isDead)
        {
            gameState = GameState::GameOver;
        }
    }
    GameBase::update();
}

void PVEGame::draw()
{
    if (gameState == GameState::Playing)
    {
        autoSnake->draw();
        // draw score
        char scoreText[20];
        sprintf_s(scoreText, "AI Score: %d", autoScore);
        DrawText(scoreText, 10, 30, 20, BLACK);
    }
    else if (gameState == GameState::Paused)
    {
        char scoreText[20];
        sprintf_s(scoreText, "AI Score: %d", autoScore);
        DrawText(scoreText, 10, 30, 20, BLACK);
    }
    GameBase::draw();
}

void PVEGame::restart()
{
    GameBase::restart();
    autoScore = 0;
    autoSnake = std::make_unique<AutoSnake>(BLUE,BLACK, config, Vector2{
                                                static_cast<float>(config.gridWidth),
                                                static_cast<float>(config.gridHeight / 2)
                                            });
    autoSnake->autoMove(food);
    snake = std::make_unique<
        PlayerSnake>(RED,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}
