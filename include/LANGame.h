#pragma once
#include "GameBase.h"
#include "TextInput.h"
#include "../lib/rapidjson/document.h"
#include "../lib/rapidjson/writer.h"
#include "../lib/rapidjson/stringbuffer.h"

#include <raylib.h>
#define ShowCursor RayShowCursor
#define CloseWindow RayCloseWindow
#include <asio.hpp>

#include "UdpClient.h"
#include "UdpServer.h"

class LANGame final : public GameBase
{
public:
    LANGame();
    ~LANGame() override;
    LANGame(const GameConfig& config, SnakeBase& snake);
    void update() override;
    void draw() override;

protected:
    void restart() override;
    TextInput textInput;
private:
    asio::io_context context;
    UdpServer server;
    UdpClient client;
    bool isHost = false;
    bool canDrawLocalSnake = false;
    bool canDrawRemoteSnake = false;
    std::unique_ptr<SnakeBase> remoteSnake = nullptr;
    bool isContextRun = false;
    std::string ip;
    Button guestButton;
    Button hostButton;
};
