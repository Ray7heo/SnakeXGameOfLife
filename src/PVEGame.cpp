#include "../include/PVEGame.h"


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
        snake->move();
        for (const auto& y : cells)
        {
            bool flag = false;
            for (int i = y.size() - 1; i > 0; i--)
            {
                if (y[i]->type == CellType::Edible)
                {
                    autoSnake->autoMove(y[i]->position);
                    flag = true;
                    break;
                }
            }
            if (flag) break;
        }
        if (autoSnake->body.front().x >= 0 && !autoSnake->isDead)
        {
            // Check collision with food
            const auto cell = cells[static_cast<int>(autoSnake->body.front().y)][static_cast<int>(autoSnake->body.
                front().x)];
            if (cell->type == CellType::Edible)
            {
                autoScore += 3;
                autoSnake->grow();
                cell->type = CellType::Blank;
            }
            else if (cell->type == CellType::Rot)
            {
                autoScore += 1;
                autoSnake->shrink();
                cell->type = CellType::Blank;
            }
            else if (cell->type == CellType::Wall)
            {
                autoSnake->isDead = true;
            }
        }
    }
    GameBase::update();
}

void PVEGame::draw()
{
    if (gameState == GameState::Playing)
    {
        if (!autoSnake->isDead)
        {
            autoSnake->draw();
        }
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
    snake = std::make_unique<
        PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}

int PVEGame::countLiveNeighbors(int x, int y) const
{
    int count = 0;
    for (int i = x - 1; i <= x + 1; ++i)
    {
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (i >= 0 && i < config.gridHeight && j >= 0 && j < config.gridWidth && !(i == x && j == y))
            {
                const auto head = snake->body.front();
                const auto tail = snake->body.back();
                if ((head.x == i && head.y == j) || (tail.x == i && tail.y == j))
                {
                    count++;
                }
            }
        }
    }
    return count + GameBase::countLiveNeighbors(x, y);
}
