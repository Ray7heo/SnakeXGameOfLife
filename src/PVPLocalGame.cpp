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
        if (rightSnake->isDead)
        {
            gameState = GameState::GameOver;
            return;
        }
        if (rightSnake->body.front().x >= 0)
        {
            // Check collision with food
            const auto cell = cells[static_cast<int>(rightSnake->body.front().y)][static_cast<int>(rightSnake->body.
                front().x)];
            if (cell->type == CellType::Edible)
            {
                rightScore += 3;
                rightSnake->grow();
                cell->type = CellType::Blank;
            }
            else if (cell->type == CellType::Rot)
            {
                rightScore += 1;
                rightSnake->shrink();
                cell->type = CellType::Blank;
            }
            else if (cell->type == CellType::Wall)
            {
                rightSnake->isDead = true;
            }
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
    rightSnake = std::make_unique<PlayerSnake>(rightSnake->headColor, rightSnake->tailColor, config, Vector2{
                                                   static_cast<float>(config.gridWidth - 1),
                                                   static_cast<float>(config.gridHeight / 2)
                                               }, true);
    snake = std::make_unique<
        PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)},
                     false);
}

int PVPLocalGame::countLiveNeighbors(const int x, const int y) const
{
    int count = 0;
    for (int i = x - 1; i <= x + 1; ++i)
    {
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (i >= 0 && i < config.gridHeight && j >= 0 && j < config.gridWidth && !(i == x && j == y))
            {
                const auto head = rightSnake->body.front();
                const auto tail = rightSnake->body.back();
                if ((head.x == i && head.y == j) || (tail.x == i && tail.y == j))
                {
                    count++;
                }
            }
        }
    }
    return count + GameBase::countLiveNeighbors(x, y);
}
