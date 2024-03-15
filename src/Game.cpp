#include "../include/Game.h"

#include "../include/AutoSnake.h"

// no arg
Game::Game() : snake(std::make_unique<PlayerSnake>(GREEN, config)), food({400, 200}),
               gameState(GameState::Start),
               startButton({400 - 100, 225 - 25, 200, 50}, GREEN, BLACK, "Start"),
               pauseButton({static_cast<float>(config.screenWidth) - 120, 10, 100, 30},GREEN, BLACK, "Pause"),
               restartButton({
                                 static_cast<float>(config.screenWidth) / 2 - 100,
                                 static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                             },GREEN,BLACK, "Restart"),
               menuButton({
                              static_cast<float>(config.screenWidth) / 2 - 100,
                              static_cast<float>(config.screenHeight) / 2 - 25 + restartButton.bounds.height,
                              200, 50
                          }, GREEN, BLACK, "Menu"),
               score(0)

{
}

Game::Game(const Config& config, SnakeBase& snake): config(config),
                                                    snake(&snake),
                                                    food({
                                                        static_cast<float>(config.screenWidth) / 2,
                                                        static_cast<float>(config.screenHeight) / 2
                                                    }),
                                                    gameState(GameState::Start),
                                                    startButton({400 - 100, 225 - 25, 200, 50}, GREEN, BLACK, "Start"),
                                                    pauseButton({
                                                                    static_cast<float>(config.screenWidth) - 120, 10,
                                                                    100, 30
                                                                },
                                                                GREEN, BLACK, "Pause"),
                                                    restartButton({
                                                                      static_cast<float>(config.screenWidth) / 2 - 100,
                                                                      static_cast<float>(config.screenHeight) / 2 - 25,
                                                                      200,
                                                                      50
                                                                  },GREEN,BLACK, "Restart"),
                                                    menuButton({
                                                                   static_cast<float>(config.screenWidth) / 2 - 100,
                                                                   static_cast<float>(config.screenHeight) / 2 - 25 +
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
        snake->move();
    // auto v = Vector2{food.x, food.y};
    // snake->autoMove(v);

    // Check collision with food
        if (CheckCollisionRecs(snake->getCollisionRec(), {
                                   food.x, food.y, static_cast<float>(config.squareSize),
                                   static_cast<float>(config.squareSize)
                               }))
        {
            score++;
            snake->grow();
            spawn_food();
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
            DrawRectangle(food.x, food.y, config.squareSize, config.squareSize, GREEN);
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
                     config.screenWidth / 2 - MeasureText("Pause !", 30) / 2,
                     config.screenHeight / 2 - 20, 30, RED);
            pauseButton.text = "resume";
            pauseButton.draw();
            menuButton.draw();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pauseButton.isClicked(GetMousePosition()))
            {
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                snake = std::make_unique<PlayerSnake>(snake->color, config);
                score = 0;
                gameState = GameState::Start;
            }
            break;
        }
    case GameState::GameOver:
        {
            restartButton.draw();
            menuButton.draw();
            DrawText("GameOver !",
                     config.screenWidth / 2 - MeasureText("GameOver !", 30) / 2,
                     config.screenHeight / 2 - restartButton.bounds.height, 30, RED);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && restartButton.isClicked(GetMousePosition()))
            {
                snake = std::make_unique<PlayerSnake>(snake->color, config);
                score = 0;
                gameState = GameState::Playing;
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && menuButton.isClicked(GetMousePosition()))
            {
                snake = std::make_unique<PlayerSnake>(snake->color, config);
                score = 0;
                gameState = GameState::Start;
            }
            break;
        }
    }
    EndDrawing();
}

void Game::spawn_food()
{
    food.x = GetRandomValue(0, config.screenWidth / config.squareSize - 1) * config.squareSize;
    food.y = GetRandomValue(0, config.screenHeight / config.squareSize - 1) * config.squareSize;
}
