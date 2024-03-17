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
                       cells(std::vector<Cell>())

{
    randomCell();

    std::thread t([&]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        for (auto& cell : cells)
        {
            if (cell.type == CellType::Edible)
            {
                cell.type = CellType::Rot;
            }
        }
    });
    t.join();
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
    cells(std::vector<Cell>())

{
    randomCell();
    std::thread t([&]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        for (auto& cell : cells)
        {
            if (cell.type == CellType::Edible)
            {
                cell.type = CellType::Rot;
            }
        }
    });
    t.detach();
}



void GameBase::update()
{
    if (gameState == GameState::Playing)
    {
        // Check collision with food
        const auto it = std::find_if(cells.begin(), cells.end(), FindCellByPosition(Vector2{snake->body.front()}));
        if (it != cells.end())
        {
            if (it->type == CellType::Edible)
            {
                score += 3;
                snake->grow();
                cells.erase(it);
            }
            else if (it->type == CellType::Rot)
            {
                score += 1;
                snake->shrink();
                cells.erase(it);
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
            for (const auto& cell : cells)
            {
                cell.draw();
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

GameBase::FindCellByPosition::FindCellByPosition(Vector2 targetPos): targetPos(targetPos)
{
}

bool GameBase::FindCellByPosition::operator()(const Cell& cell) const
{
    return cell.position.x == targetPos.x && cell.position.y == targetPos.y;
}

GameBase::FindCellByType::FindCellByType(const CellType& type): type(type)
{
}

bool GameBase::FindCellByType::operator()(const Cell& cell) const
{
    return type == cell.type;
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
        {CellType::Blank, 40},
        {CellType::Edible, 20},
        {CellType::Rot, 20},
        {CellType::Die, 15},
        {CellType::Wall, 5},
    };

    for (int i = 0; i < config.gridHeight * 3; ++i)
    {
        int totalWeight = 0;
        for (const auto& cell : weightedCells)
        {
            totalWeight += cell.weight;
        }
        // 生成随机数
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(1, totalWeight);
        const int randomNumber = dist(gen);

        int cumulativeWeight = 0;
        for (const auto& cell : weightedCells)
        {
            cumulativeWeight += cell.weight;
            if (randomNumber <= cumulativeWeight)
            {
                cells.emplace_back(cell.type, randomCellPosition(), config);
            }
        }
        cells.emplace_back(CellType::Blank, randomCellPosition(), config);
    }
}

Vector2 GameBase::randomCellPosition()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> xDist(5, config.gridWidth - 5);
    std::uniform_int_distribution<unsigned int> yDist(5, config.gridHeight - 5);

    const auto randomPos = Vector2{static_cast<float>(xDist(gen)), static_cast<float>(yDist(gen))};

    for (const auto& cell : cells)
    {
        if (cell.position.x == randomPos.x && cell.position.y == randomPos.y)
        {
            return randomCellPosition();
        }
    }
    return randomPos;
}
