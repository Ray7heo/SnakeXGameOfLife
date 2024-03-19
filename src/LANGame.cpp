﻿#include "../include/LANGame.h"

#include "../include/UdpClient.h"
#include "../include/UdpServer.h"


LANGame::LANGame(): textInput(), server(context, ""), client(context, "", ""),
                    guestButton({0, textInput.bounds.height, 200, 50}, "Guest"),
                    hostButton({0, guestButton.bounds.y + 50, 200, 50}, "Host")
{
}

LANGame::~LANGame()
{
    context.stop();
    isContextRun = false;
}

LANGame::LANGame(const GameConfig& config, SnakeBase& snake): GameBase(config, snake),
                                                              textInput({0, 0, 200, 30}, ""), server(context, "0000"),
                                                              client(context, "127.0.0.1", "12345"),
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
        remoteSnake->move();
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D))
        {
            rapidjson::Document document(rapidjson::kObjectType);
            rapidjson::Value value = snake->toJson(document.GetAllocator());
            document.AddMember("snake", value, document.GetAllocator());
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            document.Accept(writer);
            const char* data = buffer.GetString();
            if (isHost)
            {
                server.send(*new std::string(data));
            }
            else
            {
                client.send(*new std::string(data));
            }
        }
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
        if (canDrawRemoteSnake)
        {
            remoteSnake->draw();
        }
        if (canDrawLocalSnake)
        {
            snake->draw();
        }
        if (!canDrawLocalSnake && !canDrawRemoteSnake)
        {
            hostButton.draw();
            guestButton.draw();
            textInput.draw();
            textInput.update();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && startButton.isClicked(GetMousePosition()))
        {
            std::thread([this]()
            {
                asio::io_context tempContext;
                rapidjson::Document document;
                if (isHost)
                {
                    auto tempServer = UdpServer(tempContext, "12344");
                    while (!isMapReady)
                    {
                    }
                    document = rapidjson::Document(rapidjson::kObjectType);
                    rapidjson::Value gridValue = Cell::toJsons(cells, document.GetAllocator());
                    document.AddMember("cells", gridValue, document.GetAllocator());
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    document.Accept(writer);
                    tempServer.send(*new std::string(buffer.GetString()));
                }
                else
                {
                    UdpClient(tempContext, "127.0.0.1", "12344").receive([&](const std::string& data)
                    {
                        document.Parse(data.c_str());
                        std::cout << data << "\n";
                    });
                }
                tempContext.run_one();
            }).detach();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hostButton.isClicked(GetMousePosition()))
        {
            if (!isContextRun)
            {
                isHost = true;
                isContextRun = true;
                canDrawLocalSnake = true;
                std::thread([this]
                {
                    server = UdpServer(context, "12345");
                    server.receive([&](const std::string& data)
                    {
                        rapidjson::Document document;
                        // first time
                        if (remoteSnake == nullptr)
                        {
                            // send
                            document = rapidjson::Document(rapidjson::kObjectType);
                            rapidjson::Value value = snake->toJson(document.GetAllocator());
                            document.AddMember("snake", value, document.GetAllocator());
                            rapidjson::StringBuffer buffer;
                            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                            document.Accept(writer);
                            server.send(*new std::string(buffer.GetString()));
                        }
                        // parse remote snake
                        document.Parse(data.c_str());
                        remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
                        remoteSnake->config = config;
                        canDrawRemoteSnake = true;
                    });
                    context.run();
                }).detach();
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && guestButton.isClicked(GetMousePosition()))
        {
            if (!isContextRun)
            {
                isHost = false;
                canDrawLocalSnake = true;
                isContextRun = true;
                std::thread([this]
                {
                    client = UdpClient(context, "127.0.0.1", "12345");

                    rapidjson::Document document(rapidjson::kObjectType);
                    snake = std::make_unique<PlayerSnake>(snake->headColor, snake->tailColor, config, Vector2{
                                                              static_cast<float>(config.gridWidth) - 1,
                                                              static_cast<float>(config.gridHeight) / 2
                                                          });
                    rapidjson::Value value = snake->toJson(document.GetAllocator());
                    document.AddMember("snake", value, document.GetAllocator());
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    document.Accept(writer);

                    const auto data = std::make_unique<std::string>(buffer.GetString());
                    client.send(*data);

                    client.receive([&](const std::string& data)
                    {
                        document = rapidjson::Document();
                        document.Parse(data.c_str());
                        remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
                        remoteSnake->config = config;
                        canDrawRemoteSnake = true;
                    });
                    context.run();
                }).detach();
            }
        }
    }
    else if (gameState == GameState::Playing)
    {
        remoteSnake->draw();
    }
    GameBase::draw();
}
