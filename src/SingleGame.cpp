﻿#include "../include/SingleGame.h"


SingleGame::SingleGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake),
                                                                    textInput({
                                                                                  static_cast<float>(config.screenWidth)
                                                                                  / 2 - 100,
                                                                                  0, 200, 30
                                                                              }, "")
{
    loadFile();
}

SingleGame::SingleGame(): textInput({static_cast<float>(config.screenWidth) / 2 - 100, 0, 200, 30}, "")
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
        DrawText("Top Score:", 0, 0, 30, BLACK);
        int end = 0;
        for (const auto& pairs : scores)
        {
            if (end >= 10) return;
            end++;
            for (const auto& value : pairs)
            {
                auto text = value.first + ":" + std::to_string(value.second);
                DrawText(text.c_str(), 0, 30 * end, 30,RED);
            }
        }
    }
    else if (gameState == GameState::GameOver)
    {
        textInput.draw();
        textInput.update();
        const auto button = Button({static_cast<float>(config.screenWidth) / 2 - 100, 30, 200, 50}, "Save Name");
        button.draw();
        if (button.isClicked())
        {
            std::ofstream file("data.txt", std::ios::app);
            const std::string data = textInput.text + ":" + std::to_string(score) + "\n";
            if (file.is_open())
            {
                file << data;
                file.close();
            }
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
    return std::greater<std::string>()(b.begin()->first, a.begin()->first);
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
