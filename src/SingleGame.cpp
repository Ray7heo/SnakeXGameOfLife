#include "../include/SingleGame.h"

#include "../include/TextInput.h"

SingleGame::SingleGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake)
{
}

SingleGame::SingleGame()
{
    snake = std::make_unique<PlayerSnake>(RED,BLUE, config, Vector2{1, static_cast<float>(config.gridHeight / 2)});
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
    if (gameState == GameState::GameOver)
    {
        auto textInput = TextInput({static_cast<float>(config.screenWidth) / 2 - 100, 0, 200, 30}, "");
        textInput.draw();
        textInput.update();
        const auto button = Button({static_cast<float>(config.screenWidth) / 2 - 100, 30, 200, 50}, "Save Name");
        if (button.isClicked())
        {
           std::cout << textInput.text << "\n"; 
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
