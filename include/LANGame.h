#pragma once
#include "GameBase.h"
#include "TextInput.h"

#include <raylib.h>
#define ShowCursor RayShowCursor
#define CloseWindow RayCloseWindow
#include <asio.hpp>

class LANGame final : public GameBase
{
public:
    LANGame();

    LANGame(const GameConfig& config, SnakeBase& snake);
    void update() override;
    void draw() override;

protected:
    void restart() override;
    TextInput textInput;
private:
    asio::io_context context;
    bool isContextRun = false;
    asio::ip::udp::socket socket;
    std::string ip;
    Button guestButton;
    Button hostButton;
};
