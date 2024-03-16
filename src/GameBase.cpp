#include "../include/GameBase.h"
#include "../include/AutoSnake.h"

// no arg
GameBase::GameBase() : config(Config()), gameState(GameState::Start),
                       score(0), food(),
                       startButton({
                                       static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize) -
                                       100,
                                       static_cast<float>(15 * config.tileSize) - 25, 200, 50
                                   }, "Start"),
                       pauseButton({
                                       static_cast<float>(config.gridWidth * config.tileSize) - 120, 10, 100, 30
                                   }, "Pause"),
                       restartButton({
                                         static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize)
                                         - 100,
                                         static_cast<float>(15 * config.tileSize) - 25, 200, 50
                                     }, "ReStart"),
                       menuButton({
                                      static_cast<float>(config.gridWidth * config.tileSize) / 2 - 100,
                                      static_cast<float>(config.gridHeight * config.tileSize) / 2 + restartButton.
                                      bounds.
                                      height,
                                      200, 50
                                  }, "Menu")

{
    spawnFood();
}


GameBase::GameBase(const Config& config, SnakeBase& snake):
    config(config), gameState(GameState::Start), score(0),
    snake(&snake),
    startButton({
                    static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize) - 100,
                    static_cast<float>(15 * config.tileSize) - 25, 200, 50
                }, "Start"),
    pauseButton({static_cast<float>(config.gridWidth * config.tileSize) - 120, 10, 100, 30},GREEN, BLACK, "Pause"),
    restartButton({
                      static_cast<float>(config.gridWidth) / 2 - 100, static_cast<float>(config.gridHeight) / 2 - 25,
                      200, 50
                  }, "Restart"),
    menuButton({
                   static_cast<float>(config.gridWidth) / 2 - 100,
                   static_cast<float>(config.gridHeight) / 2 - 25 + restartButton.bounds.y, 200, 50
               }, "Menu")

{
    spawnFood();
}


void GameBase::update()
{
    if (gameState == GameState::Playing)
    {
        // Check collision with food
        if (CheckCollisionRecs(snake->getCollisionRec(), {
                                   food.x * config.tileSize, food.y * config.tileSize,
                                   static_cast<float>(config.tileSize),
                                   static_cast<float>(config.tileSize)
                               }))
        {
            score++;
            snake->grow();
            spawnFood();
        }

        if (snake->isDead)
        {
            gameState = GameState::GameOver;
        }
    }
}

void GameBase::draw()
{
    // BeginDrawing();
    //
    // ClearBackground(RAYWHITE);

    switch (gameState)
    {
    case GameState::Start:
        {
            startButton.draw();
            menuButton.draw();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && startButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Menu;
            }
            break;
        }
    case GameState::Playing:
        {
            // draw food
            DrawRectangle(food.x * config.tileSize, food.y * config.tileSize, config.tileSize, config.tileSize, GREEN);
            // draw snake
            snake->draw();
            // draw score
            char scoreText[20];
            sprintf_s(scoreText, "You Score: %d", score);
            DrawText(scoreText, 10, 10, 20, BLACK);
            // draw button
            pauseButton.text = "pause";
            pauseButton.draw();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pauseButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Paused;
            }
            break;
        }
    case GameState::Paused:
        {
            // draw score
            char scoreText[20];
            sprintf_s(scoreText, "Score: %d", score);
            DrawText(scoreText, 10, 10, 20, BLACK);
            DrawText("Pause !",
                     config.gridWidth * config.tileSize / 2 - MeasureText("Pause !", 30) / 2,
                     config.gridHeight * config.tileSize / 2 - 20, 30, RED);
            pauseButton.text = "resume";
            pauseButton.draw();
            menuButton.draw();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pauseButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Menu;
            }
            break;
        }
    case GameState::GameOver:
        {
            restartButton.draw();
            menuButton.draw();
            DrawText("GameOver !",
                     config.gridWidth / 2 * config.tileSize - MeasureText("GameOver !", 30) / 2,
                     config.gridHeight / 2 * config.tileSize - restartButton.bounds.height, 30, RED);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && restartButton.isClicked(GetMousePosition()))
            {
                restart();
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                restart();
                gameState = GameState::Menu;
            }
            break;
        }
    }
    // EndDrawing();
}

void GameBase::spawnFood()
{
    food.x = GetRandomValue(0, config.gridWidth - 1);
    food.y = GetRandomValue(0, config.gridHeight - 1);
}

void GameBase::restart()
{
    spawnFood();
    score = 0;
}
