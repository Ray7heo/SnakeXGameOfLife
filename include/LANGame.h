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
    bool canDrawServerSnake = false;
    bool canDrawClientSnake = false;
    std::unique_ptr<SnakeBase> remoteSnake;
    asio::io_context context;
    bool isContextRun = false;
    asio::ip::udp::socket socket;
    std::string ip;
    Button guestButton;
    Button hostButton;
};
