#pragma once
#include <thread>
#include "Config.h"
#include "../lib/RayLib/raylib.h"
#include "../include/Config.h"
#include "../include/SingleGame.h"
#include "../include/PVPLocalGame.h"
#include "../include/PVEGame.h"
#include "../include/LANGame.h"
#include "../include/GameBase.h"

class Menu
{
public:
    explicit Menu(const Config& config);
    void update();

private:
    Config config;
    std::unique_ptr<GameBase> game;
    Button singleGameButton;
    Button pveGameButton;
    Button pvpLocalGameButton;
    Button lanGameButton;

    void drawGame() const;
    void selectGameMode();
};
