#include "../include/Game.h"
#include "../include/AutoSnake.h"

// no arg
Game::Game() : config(Config()), snake(std::make_unique<AutoSnake>(RED, config)),
               food({static_cast<float>(config.gridWidth) / 2, static_cast<float>(config.gridHeight) / 2}),
               gameState(GameState::Start),
               startButton({
                               static_cast<float>(config.gridWidth) / 2 * config.tileSize - 100,
                               static_cast<float>(15 * config.tileSize) - 25, 200, 50
                           }, GREEN, BLACK, "Start"),
               pauseButton({static_cast<float>(config.gridWidth) - 120, 10, 100, 30},GREEN, BLACK, "Pause"),
               restartButton({
                                 static_cast<float>(config.gridWidth) / 2 - 100,
                                 static_cast<float>(config.gridHeight) / 2 - 25, 200, 50
                             },GREEN,BLACK, "Restart"),
               menuButton({
                              static_cast<float>(config.gridWidth) / 2 - 100,
                              static_cast<float>(config.gridHeight) / 2 - 25 + restartButton.bounds.height,
                              200, 50
                          }, GREEN, BLACK, "Menu"),
               score(0)

{
}


Game::Game(const Config& config, SnakeBase& snake): config(config),
                                                    snake(&snake),
                                                    food({
                                                        static_cast<float>(config.gridWidth) / 2,
                                                        static_cast<float>(config.gridHeight) / 2
                                                    }),
                                                    gameState(GameState::Start),
                                                    startButton({
                                                                    static_cast<float>(25 * config.tileSize) - 100,
                                                                    static_cast<float>(15 * config.tileSize) - 25, 200,
                                                                    50
                                                                }, GREEN, BLACK, "Start"),
                                                    pauseButton({
                                                                    static_cast<float>(config.gridWidth) - 120, 10,
                                                                    100, 30
                                                                },
                                                                GREEN, BLACK, "Pause"),
                                                    restartButton({
                                                                      static_cast<float>(config.gridWidth) / 2 - 100,
                                                                      static_cast<float>(config.gridHeight) / 2 - 25,
                                                                      200,
                                                                      50
                                                                  },GREEN,BLACK, "Restart"),
                                                    menuButton({
                                                                   static_cast<float>(config.gridWidth) / 2 - 100,
                                                                   static_cast<float>(config.gridHeight) / 2 - 25 +
                                                                   restartButton.bounds.y,
                                                                   200,
                                                                   50
                                                               }, GREEN, BLACK, "Menu"),
                                                    score(0)

{
}


void Game::update()
{
    switch (gameState)
    {
    case GameState::Start:
        break;
    case GameState::Playing:
        // snake->move();
        auto v = Vector2{food.x, food.y};
        snake->autoMove(v);

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
        break;
    case GameState::Paused:
        break;
    case GameState::GameOver:
        break;
    }
}

void Game::draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (gameState)
    {
    case GameState::Start:
        {
            startButton.draw();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && startButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Playing;
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
            sprintf_s(scoreText, "Score: %d", score);
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
                     config.gridWidth / 2 - MeasureText("Pause !", 30) / 2,
                     config.gridHeight / 2 - 20, 30, RED);
            pauseButton.text = "resume";
            pauseButton.draw();
            menuButton.draw();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pauseButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                spawnFood();
                gameState = GameState::Start;
            }
            break;
        }
    case GameState::GameOver:
        {
            restartButton.draw();
            menuButton.draw();
            DrawText("GameOver !",
                     config.gridWidth - MeasureText("GameOver !", 30) / 2,
                     config.gridHeight - restartButton.bounds.height, 30, RED);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && restartButton.isClicked(GetMousePosition()))
            {
                restart();
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                restart();
                gameState = GameState::Start;
            }
            break;
        }
    }
    EndDrawing();
}

void Game::spawnFood()
{
    food.x = GetRandomValue(0, config.gridWidth - 1);
    food.y = GetRandomValue(0, config.gridHeight - 1);
}

void Game::restart()
{
    spawnFood();
    snake = std::make_unique<AutoSnake>(snake->color, config);
    score = 0;
}
