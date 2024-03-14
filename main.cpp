#include "include/Config.h"
#include "include/Game.h"
#include "lib/RayLib/raylib.h"


int main(int argc, char* argv[])
{
    const auto config = Config();
    InitWindow(config.screenWidth, config.screenHeight, "Snake Game");
    auto game = Game({400, 200}, config);
    SetTargetFPS(10);

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }

    CloseWindow();

    return 0;
}
