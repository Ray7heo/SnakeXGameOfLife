#include "../include/GameBase.h"

// no arg
GameBase::GameBase() : config(GameConfig()), gameState(GameState::Start),
                       score(0),
                       startButton({
                                       static_cast<float>(config.screenWidth) / 2 - 100,
                                       static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                                   }, "Start"),
                       pauseButton({
                                       static_cast<float>(config.screenWidth) - 120, 0, 100, 30
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
    randomCell();
}

GameBase::GameBase(const GameConfig& config, SnakeBase& snake):
    config(config), gameState(GameState::Start), score(0), snake(&snake),
    startButton({
                    static_cast<float>(config.screenWidth) / 2 - 100,
                    static_cast<float>(config.screenHeight) / 2 - 25, 200, 50
                }, "Start"),
    pauseButton({
                    static_cast<float>(config.screenWidth) - 120, 0, 100, 30
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
                                                              config.gridHeight))),
    camera({{0, 0}, {0, 0}, 0, 1.0f})

{
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    randomCell();
}

void GameBase::update()
{
    if (gameState == GameState::Playing)
    {
        if (snake->isDead)
        {
            gameState = GameState::GameOver;
            return;
        }
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
            else if (cell->type == CellType::Wall)
            {
                snake->isDead = true;
            }
        }
    }
}

bool isDragging = false;
Vector2 dragStartPosition = {0};

void GameBase::dragCamera()
{
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (!isDragging)
        {
            // 记录鼠标按下时的位置
            dragStartPosition = GetMousePosition();
            isDragging = true;
        }
        else
        {
            // 获取鼠标当前位置
            Vector2 dragEndPosition = GetMousePosition();

            // 计算拖动的偏移量
            Vector2 dragOffset;
            dragOffset.x = dragEndPosition.x - dragStartPosition.x;
            dragOffset.y = dragEndPosition.y - dragStartPosition.y;

            // 更新摄像机位置
            camera.target.x -= dragOffset.x / 2;
            camera.target.y -= dragOffset.y / 2;

            // 更新拖动起始位置
            dragStartPosition = dragEndPosition;
        }
    }
    else
    {
        // 鼠标释放时停止拖动
        isDragging = false;
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
            if (startButton.isClicked())
            {
                gameState = GameState::Playing;
            }
            if (menuButton.isClicked())
            {
                gameState = GameState::Menu;
            }
            break;
        }
    case GameState::Playing:
        {
            dragCamera();
            // draw score
            char scoreText[20];
            sprintf_s(scoreText, "You Score: %d", score);
            DrawText(scoreText, 10, 10, 20, BLACK);
            // draw button
            pauseButton.text = "pause";
            pauseButton.draw();
            if (pauseButton.isClicked())
            {
                snake->direction = {0, 0};
                gameState = GameState::Paused;
            }

            BeginMode2D(camera);
            {
                const float scroll = GetMouseWheelMove();
                if (camera.zoom > 0)
                {
                    camera.zoom += scroll * 0.1f;
                }
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
                if (!snake->isDead)
                {
                    snake->draw();
                }
            }
            EndMode2D();
            break;
        }
    case GameState::Paused:
        {
            dragCamera();
            // draw score
            char scoreText[20];
            sprintf_s(scoreText, "You Score: %d", score);
            DrawText(scoreText, 10, 10, 20, BLACK);

            DrawText("Pause !",
                     config.screenWidth / 2 - MeasureText("Pause !", 30) / 2,
                     config.screenHeight / 2 - 20, 30, BLACK);
            pauseButton.text ="resume";
            pauseButton.draw();
            menuButton.draw();
            if (pauseButton.isClicked())
            {
                gameState = GameState::Playing;
            }
            if (menuButton.isClicked())
            {
                gameState = GameState::Menu;
            }
            BeginMode2D(camera);
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
                if (!snake->isDead)
                {
                    snake->draw();
                }
            }
            EndMode2D();

            break;
        }
    case GameState::GameOver:
        {
            // draw score
            char scoreText[20];
            sprintf_s(scoreText, "You Score: %d", score);
            DrawText(scoreText, 0, 0, 20, BLACK);

            restartButton.draw();
            menuButton.draw();
            DrawText("GameOver !",
                     config.screenWidth / 2 - MeasureText("GameOver !", 30) / 2,
                     config.screenHeight / 2 - restartButton.bounds.height, 30, RED);
            if (restartButton.isClicked())
            {
                restart();
                gameState = GameState::Playing;
            }
            if (menuButton.isClicked())
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

[[noreturn]] void GameBase::updateCell() const
{
    while (true)
    {
        if (gameState != GameState::Playing)
        {
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for (int y = 0; y < config.gridHeight; y++)
        {
            for (int x = 0; x < config.gridWidth; x++)
            {
                const auto cell = cells[y][x];
                const auto count = countLiveNeighbors(x, y);
                if (cell->type == CellType::Edible || cell->type == CellType::Rot)
                {
                    if (count < 2 || count > 3)
                    {
                        cell->type = CellType::Die;
                        cell->reductionCounter();
                    }
                }
                else if (cell->type == CellType::Die)
                {
                    if (count == 3)
                    {
                        cell->resurgence();
                    }
                }
            }
        }
    }
}

void GameBase::randomCell()
{
    cells.clear();

    // 计算权重总和
    const std::vector<WeightedCell> weightedCells = {
        {CellType::Blank, 400},
        {CellType::Edible, 50},
        {CellType::Rot, 20},
        {CellType::Die, 15},
        {CellType::Wall, 5},
    };

    for (int y = 0; y < config.gridHeight; y++)
    {
        cells.emplace_back();
        for (int x = 0; x < config.gridWidth; x++)
        {
            if (x == 0 || x == config.gridWidth - 1)
            {
                cells[y].push_back(std::make_shared<Cell>(CellType::Blank,
                                                          Vector2{static_cast<float>(x), static_cast<float>(y)},
                                                          config));
            }
            else
            {
                cells[y].push_back(std::make_shared<Cell>(Cell::randomType(weightedCells),
                                                          Vector2{static_cast<float>(x), static_cast<float>(y)},
                                                          config));
            }
        }
    }
    isMapReady = true;
    updateThread = std::thread(&GameBase::updateCell, this);
    updateThread.detach();
}

int GameBase::countLiveNeighbors(const int x, const int y) const
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
                if (cells[j][i]->type == CellType::Edible || cells[j][i]->type == CellType::Rot || (head.x == i && head.
                    y == j) || (tail.x == i && tail.y == j))
                {
                    count++;
                }
            }
        }
    }
    return count;
}
