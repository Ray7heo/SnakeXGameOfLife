#include "../include/LANGame.h"

#include "../include/UdpClient.h"
#include "../include/UdpServer.h"


LANGame::LANGame(): textInput(), socket(context), guestButton({0, textInput.bounds.height, 200, 50}, "Guest"),
                    hostButton({0, guestButton.bounds.y + 50, 200, 50}, "Host")
{
}

LANGame::LANGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake),
                                                              textInput({0, 0, 200, 30}, ""),
                                                              socket(context),
                                                              guestButton(
                                                                  {0, textInput.bounds.height, 200, 50}, "Guest"),
                                                              hostButton({0, guestButton.bounds.y + 50, 200, 50},
                                                                         "Host")
{
}

void LANGame::update()
{
    if (gameState == GameState::Playing)
    {
        snake->move();
    }
    GameBase::update();
}

void LANGame::restart()
{
    GameBase::restart();
    snake = std::make_unique<
        PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{0, static_cast<float>(config.gridHeight / 2)});
}

void LANGame::draw()
{
    if (gameState == GameState::Start)
    {
        hostButton.draw();
        guestButton.draw();
        textInput.draw();
        textInput.update();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hostButton.isClicked(GetMousePosition()))
        {
            if (!isContextRun)
            {
                isContextRun = true;
                std::thread([this]()
                {
                    UdpServer server(context, "12345");
                    server.receive([&](const std::string& message)
                    {
                        std::cout << message << "\n";
                        server.send(*new std::string("Hello Client"));
                    });
                    context.run();
                }).detach();
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && guestButton.isClicked(GetMousePosition()))
        {
            if (!isContextRun)
            {
                isContextRun = true;
                std::thread([this]()
                {
                    UdpClient client(context, "127.0.0.1", "12345");
                    client.send(*new std::string("Hello Server"));
                    client.receive([&](const std::string& message)
                    {
                        std::cout << message << "\n";
                    });
                    context.run();
                }).detach();
            }
        }
    }
    GameBase::draw();
}
