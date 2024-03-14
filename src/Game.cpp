#include "../include/Game.h"

#include <iostream>


Game::Game() : snake(RED, Config()), food({400, 200}), direction({1, 0}), gameOver(false)
{
}

Game::Game(const Vector2& food, const Config& config): snake(RED, config),
                                                       food(food),
                                                       direction({1, 0}),
                                                       gameOver(false),
                                                       gameState(GameState::Playing),
                                                       config(config)
{
}

void Game::Update()
{
    if (!gameOver)
    {
        const auto onlyHead = snake.body.size() < 2;
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
        {
            if (direction.y != 1 || onlyHead)
                direction = {0, -1};
        }
        else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
        {
            if (direction.y != -1 || onlyHead)
                direction = {0, 1};
        }
        else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            if (direction.x != 1 || onlyHead)
                direction = {-1, 0};
        }
        else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            if (direction.x != -1 || onlyHead)
                direction = {1, 0};
        }

        snake.Move(direction);

        // Check collision with food
        if (CheckCollisionRecs(snake.GetCollisionRec(), {
                                   food.x, food.y, static_cast<float>(config.squareSize),
                                   static_cast<float>(config.squareSize)
                               }))
        {
            snake.Grow();
            SpawnFood();
            gameOver = false;
        }

        // Check collision with walls
        if (snake.position.x >= config.screenWidth || snake.position.x < 0 ||
            snake.position.y >= config.screenHeight || snake.position.y < 0)
        {
            gameOver = true;
            gameState = GameState::GameOver;
        }

        // Check self-collision
        if (!onlyHead)
        {
            for (size_t i = 1; i < snake.body.size(); i++)
            {
                if (snake.position.x == snake.body[i].x && snake.position.y == snake.body[i].y)
                {
                    gameOver = true;
                    gameState = GameState::GameOver;
                    break;
                }
            }
        }
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    snake.Draw();

    DrawRectangle(food.x, food.y, config.squareSize, config.squareSize, GREEN);

    if (gameOver)
    {
        DrawText("Game Over! Press ENTER to restart",
                 config.screenWidth / 2 - MeasureText("Game Over! Press ENTER to restart", 30) / 2,
                 config.screenHeight / 2 - 20, 30, RED);
    }

    EndDrawing();
}

void Game::SpawnFood()
{
    food.x = GetRandomValue(0, config.screenWidth / config.squareSize - 1) * config.squareSize;
    food.y = GetRandomValue(0, config.screenHeight / config.squareSize - 1) * config.squareSize;
}
