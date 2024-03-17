#pragma once
#include <thread>
#include <raylib.h>

#include "../include/GameConfig.h"
#include "../include/SingleGame.h"
#include "../include/PVPLocalGame.h"
#include "../include/PVEGame.h"
#include "../include/LANGame.h"
#include "../include/GameBase.h"
#include "../include/PlayerSnake.h"

class Menu
{
public:
    explicit Menu(const GameConfig& config);
    void update();

private:
    GameConfig config;
    std::unique_ptr<GameBase> game;
    Button lanGameButton;
    Button pvpLocalGameButton;
    Button pveGameButton;
    Button singleGameButton;

    void drawGame() const;
    void selectGameMode();
};
