#include "../include/GameBase.h"

// no arg
GameBase::GameBase() : config(GameConfig()), gameState(GameState::Start),
                       score(0),
                       startButton({
                                       static_cast<float>(config.screenWidth) / 2 - 100,
                                       static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                                   }, "Start"),
                       pauseButton({
                                       static_cast<float>(config.screenWidth) - 120, 10, 100, 30
                                   }, "Pause"),
                       restartButton({
                                         static_cast<float>(config.screenWidth) / 2 - 100,
                                         static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                                     }, "ReStart"),
                       menuButton({
                                      static_cast<float>(config.screenWidth) / 2 - 100,
                                      restartButton.bounds.y + restartButton.bounds.height, 200, 50
                                  }, "Menu"),
                       cells(std::vector<std::vector<std::shared_ptr<Cell>>>(config.gridWidth,
                                                                             std::vector<std::shared_ptr<Cell>>(
                                                                                 config.gridHeight)))

{
    // randomCell();
}


GameBase::GameBase(const GameConfig& config, SnakeBase& snake):
    config(config), gameState(GameState::Start), score(0),
    snake(&snake),
    startButton({
                    static_cast<float>(config.screenWidth) / 2 - 100,
                    static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                }, "Start"),
    pauseButton({
                    static_cast<float>(config.screenWidth) - 120, 10, 100, 30
                }, "Pause"),
    restartButton({
                      static_cast<float>(config.screenWidth) / 2 - 100,
                      static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                  }, "ReStart"),
    menuButton({
                   static_cast<float>(config.screenWidth) / 2 - 100,
                   restartButton.bounds.y + restartButton.bounds.height, 200, 50
               }, "Menu"),
    cells(std::vector<std::vector<std::shared_ptr<Cell>>>(config.gridWidth,
                                                          std::vector<std::shared_ptr<Cell>>(config.gridHeight)))

{
    randomCell();
}


void GameBase::update()
{
    if (gameState == GameState::Playing)
    {
        if (snake->body.front().x >= 0)
        {
            // Check collision with food
            const auto cell = cells[static_cast<int>(snake->body.front().y)][static_cast<int>(snake->body.front().x)];
            if (cell->type == CellType::Edible)
            {
                score += 3;
                snake->grow();
                cell->type = CellType::Blank;
            }
            else if (cell->type == CellType::Rot)
            {
                score += 1;
                snake->shrink();
                cell->type = CellType::Blank;
            }
        }

        if (snake->isDead)
        {
            gameState = GameState::GameOver;
        }
    }
}

void GameBase::draw()
{
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
            // draw grid
            for (int i = 0; i <= config.gridHeight; ++i)
            {
                DrawLine(0, i * config.tileSize, config.gridWidth * config.tileSize, i * config.tileSize, BLACK);
            }
            for (int i = 0; i <= config.gridWidth; ++i)
            {
                DrawLine(i * config.tileSize, 0, i * config.tileSize, config.gridHeight * config.tileSize, BLACK);
            }
            // draw cell
            for (const auto& yCell : cells)
            {
                for (const auto& xCell : yCell)
                {
                    xCell->draw();
                }
            }
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
            sprintf_s(scoreText, "You Score: %d", score);
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
}

void GameBase::restart()
{
    randomCell();
    score = 0;
}

void GameBase::randomCell()
{
    cells.clear();
    // 计算权重总和
    const std::vector<WeightedCell> weightedCells = {
        {CellType::Blank, 800},
        {CellType::Edible, 20},
        {CellType::Rot, 20},
        {CellType::Die, 15},
        {CellType::Wall, 5},
    };

    for (int y = 0; y < config.gridHeight; y++)
    {
        cells.emplace_back();
        for (int x = 0; x < config.gridWidth; x++)
        {
            cells[y].push_back(std::make_shared<Cell>(Cell::randomType(weightedCells),
                                                      Vector2{static_cast<float>(x), static_cast<float>(y)},
                                                      config));
        }
    }
}
