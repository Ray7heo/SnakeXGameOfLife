#include "include/Config.h"
#include "include/Game.h"


int main(int argc, char* argv[])
{
    const auto config = Config();
    InitWindow(config.screenWidth, config.screenHeight, "Snake Game");
    
    
    // auto game = Game({400, 200}, config);
    auto game = Game();
    SetTargetFPS(10);

    while (!WindowShouldClose())
    {
        if (game.gameState == GameState::Playing)
        {
            game.update();
        }
        game.draw();
    }

    CloseWindow();

    return 0;
}
