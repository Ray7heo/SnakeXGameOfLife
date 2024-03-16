
#include "include/PVEGame.h"
#include "include/Config.h"
#include "include/SingleGame.h"
#include "include/GameBase.h"


int main(int argc, char* argv[])
{
    const auto config = Config();
    InitWindow(config.gridWidth * config.tileSize, config.gridHeight * config.tileSize, "Snake Game");

    const std::unique_ptr<GameBase> game = std::make_unique<PVEGame>(); 
     
    SetTargetFPS(10);

    while (!WindowShouldClose())
    {
        if (game->gameState == GameState::Playing)
        {
            game->update();
        }
        game->draw();
    }

    CloseWindow();

    return 0;
}
