#include "../include/Menu.h"


Menu::Menu(const Config& config):
    config(config),
    singleGameButton({
                         static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize) - 100,
                         static_cast<float>(config.gridHeight - 8) * static_cast<float>(config.tileSize) - 30, 200, 50
                     }, "Single"),
    pveGameButton({
                      static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize) - 100,
                      static_cast<float>(config.gridHeight - 6) * static_cast<float>
                      (config.tileSize) - 30,
                      200, 50
                  }, "PVE"),
    pvpLocalGameButton({
                           static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize) - 100,
                           static_cast<float>(config.gridHeight - 4) * static_cast<float>(config.tileSize) - 30, 200, 50
                       }, "PVP Local"),
    lanGameButton({
                      static_cast<float>(config.gridWidth) / 2 * static_cast<float>(config.tileSize) - 100,
                      static_cast<float>(config.gridHeight - 2) * static_cast<float>(config.tileSize) - 25, 200, 50
                  }, "PVP LAN")
{
}

void Menu::update()
{
    InitWindow(config.gridWidth * config.tileSize, config.gridHeight * config.tileSize, "Snake X Game of Life");
    SetTargetFPS(10);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        selectGameMode();
        drawGame();

        EndDrawing();
    }

    CloseWindow();
}

void Menu::drawGame() const
{
    if (game != nullptr && game->gameState != GameState::Menu)
    {
        if (game->gameState == GameState::Playing)
        {
            game->update();
        }
        game->draw();
    }
}

void Menu::selectGameMode()
{
    if (game == nullptr || game->gameState == GameState::Menu)
    {
        singleGameButton.draw();
        pvpLocalGameButton.draw();
        pveGameButton.draw();
        lanGameButton.draw();
        DrawText("Snake X Game of Life",
                 config.gridWidth / 2 * config.tileSize - MeasureText("Snake X Game of Life", 50) / 2,
                 config.gridHeight / 2 * config.tileSize, 50, RED);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && singleGameButton.isClicked(GetMousePosition()))
        {
            game = std::make_unique<SingleGame>();
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pvpLocalGameButton.isClicked(GetMousePosition()))
        {
            game = std::make_unique<PVPLocalGame>();
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pveGameButton.isClicked(GetMousePosition()))
        {
            game = std::make_unique<PVEGame>();
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && lanGameButton.isClicked(GetMousePosition()))
        {
            game = std::make_unique<LANGame>();
        }
    }
}
