#include "../include/LANGame.h"

#define DrawText RayDrawText
#define RayDrawText DrawText
#define RayCloseWindow CloseWindow

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
    canUpdateCell = false;
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
            document.AddMember("dataType", rapidjson::StringRef("snake"), document.GetAllocator());
            document.AddMember("score", score, document.GetAllocator());
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
                if (isHost)
                {
                    auto document = rapidjson::Document(rapidjson::kObjectType);
                    document.AddMember("dataType", rapidjson::StringRef("state"), document.GetAllocator());
                    document.AddMember("state", rapidjson::StringRef("playing"), document.GetAllocator());
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    document.Accept(writer);
                    server.send(*new std::string(buffer.GetString()));
                }
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
                        document = rapidjson::Document();
                        document.Parse(data.c_str());
                        if (document["state"] == "start")
                        {
                            // parse remote snake
                            remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
                            remoteSnake->config = config;
                            canDrawRemoteSnake = true;
                            // send
                            document = rapidjson::Document(rapidjson::kObjectType);
                            rapidjson::Value value = snake->toJson(document.GetAllocator());
                            document.AddMember("snake", value, document.GetAllocator());
                            document.AddMember("score", score, document.GetAllocator());
                            document.AddMember("dataType", rapidjson::StringRef("snake"), document.GetAllocator());
                            rapidjson::StringBuffer buffer;
                            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                            document.Accept(writer);
                            server.send(*new std::string(buffer.GetString()));

                            while (!isMapReady)
                            {
                            }
                            // send map info
                            for (int y = 0; y < config.gridHeight; y++)
                            {
                                for (int x = 0; x < config.gridWidth; x++)
                                {
                                    document = rapidjson::Document(rapidjson::kObjectType);
                                    document.AddMember("dataType", rapidjson::StringRef("cells"),
                                                       document.GetAllocator());
                                    document.AddMember("x", x, document.GetAllocator());
                                    document.AddMember("y", y, document.GetAllocator());
                                    document.AddMember("cell", cells[y][x]->toJson(document.GetAllocator()),
                                                       document.GetAllocator());
                                    rapidjson::StringBuffer buffer;
                                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                                    document.Accept(writer);
                                    server.send(*new std::string(buffer.GetString()));
                                }
                            }
                        }
                        else
                        {
                            remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
                            remoteScore = document["score"].GetInt();
                        }
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
                    document.AddMember("score", score, document.GetAllocator());
                    document.AddMember("state", rapidjson::StringRef("start"), document.GetAllocator());
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    document.Accept(writer);

                    const auto data = std::make_unique<std::string>(buffer.GetString());
                    client.send(*data);

                    client.receive([&](const std::string& data)
                    {
                        document = rapidjson::Document();
                        document.Parse(data.c_str());
                        if (document["dataType"] == "snake")
                        {
                            remoteSnake = std::make_unique<SnakeBase>(SnakeBase::fromJson(document["snake"]));
                            remoteScore = document["score"].GetInt();
                            remoteSnake->config = config;
                            canDrawRemoteSnake = true;
                        }
                        else if (document["dataType"] == "cells")
                        {
                            const int x = document["x"].GetInt();
                            const int y = document["y"].GetInt();
                            const auto& cell = std::make_shared<Cell>(Cell::fromJson(document["cell"]));
                            cells[y][x]->type = cell->type;
                        }
                        else
                        {
                            gameState = GameState::Playing;
                        }
                    });
                    context.run();
                }).detach();
            }
        }
    }
    else if (gameState == GameState::Playing)
    {
        if (gameState == GameState::Playing)
        {
            remoteSnake->draw();
            // draw score
            char scoreText[30];
            sprintf_s(scoreText, "RightPlayer Score: %d", remoteScore);
            DrawText(scoreText, 10, 30, 20, BLACK);
        }
        else if (gameState == GameState::Paused)
        {
            char scoreText[30];
            sprintf_s(scoreText, "RightPlayer Score: %d", remoteScore);
            DrawText(scoreText, 10, 30, 20, BLACK);
        }
    }
    GameBase::draw();
}
