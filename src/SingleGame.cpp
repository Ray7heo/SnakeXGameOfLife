#include "../include/SingleGame.h"


SingleGame::SingleGame(const GameConfig& config, SnakeBase& snake):
    GameBase(config, snake), textInput({static_cast<float>(config.screenWidth) / 2 - 100, 0, 200, 30}, ""),
    tileNumInput({0, static_cast<float>(config.screenHeight) - 80, 100, 30}, std::to_string(config.gridWidth)),
    saveConfigButton({0, static_cast<float>(config.screenHeight) - 50, 200, 50}, "Save Cell Number")
{
    loadFile();
}

SingleGame::SingleGame(): textInput({static_cast<float>(config.screenWidth) / 2 - 100, 0, 200, 30}, ""),
                          tileNumInput({0, static_cast<float>(config.screenHeight) - 80, 100, 30}, std::to_string(config.gridWidth)),
                          saveConfigButton({0, static_cast<float>(config.screenHeight) - 50, 200, 50},
                                           "Save Cell Number")
{
    snake = std::make_unique<PlayerSnake>(RED,BLUE, config, Vector2{1, static_cast<float>(config.gridHeight / 2)});
    loadFile();
}

void SingleGame::update()
{
    if (gameState == GameState::Playing)
    {
        snake->move();
    }
    GameBase::update();
}

void SingleGame::draw()
{
    if (gameState == GameState::Start)
    {
        saveConfigButton.draw();
        tileNumInput.draw();
        tileNumInput.update();
    }
    else if (gameState == GameState::GameOver)
    {
        textInput.draw();
        textInput.update();

        const auto button = Button({static_cast<float>(config.screenWidth) / 2 - 100, 30, 200, 50}, "Save Name");
        button.draw();

        DrawText("Top Score:", 0, 20, 30, BLACK);
        int end = 0;
        for (const auto& pairs : scores)
        {
            if (end >= 10) return;
            end++;
            for (const auto& value : pairs)
            {
                auto text = value.first + ":" + std::to_string(value.second);
                DrawText(text.c_str(), 0, 30 * end + 30, 30,RED);
            }
        }
        if (button.isClicked())
        {
            scores.insert(std::map<std::string, int>{
                {textInput.text, score}
            });
            std::ofstream file("data.txt", std::ios::app);
            const std::string data = textInput.text + ":" + std::to_string(score) + "\n";
            if (file.is_open())
            {
                file << data;
                file.close();
            }
        }
        if (saveConfigButton.isClicked())
        {
            const auto cellNum = std::stoi(tileNumInput.text);
            config.gridHeight = cellNum;
            config.gridWidth = cellNum;
            config.tileSize = config.screenHeight / cellNum;
            snake = std::make_unique<
                PlayerSnake>(snake->headColor, snake->tailColor, config,
                             Vector2{0, static_cast<float>(config.gridHeight / 2)});
        }
    }
    GameBase::draw();
}

void SingleGame::restart()
{
    GameBase::restart();
    snake = std::make_unique<
        PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}

bool SingleGame::CompareMaps::operator()(const std::map<std::string, int>& a, const std::map<std::string, int>& b) const
{
    for (const auto& pair : a)
    {
        if (b.find(pair.first) == b.end() || b.at(pair.first) != pair.second)
        {
            return a > b;
        }
    }
    return false;
}

void SingleGame::loadFile()
{
    std::ifstream file("data.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            auto playerName = line.substr(0, line.find(':'));
            auto playerScore = std::stoi(line.substr(line.find(':') + 1));
            std::map<std::string, int> map = {{playerName, playerScore}};
            scores.insert(map);
        }
        file.close();
    }
}
