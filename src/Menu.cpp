#include "../include/Menu.h"

Menu::Menu(const GameConfig& config):
    config(config),
    lanGameButton({
                      static_cast<float>(config.screenWidth) / 2 - 100,
                      static_cast<float>(config.screenHeight) - 50, 200, 50
                  }, "PVP LAN"),
    pvpLocalGameButton({
                           static_cast<float>(config.screenWidth) / 2 - 100,
                           lanGameButton.bounds.y - 50, 200, 50
                       }, "PVP Local"),
    pveGameButton({
                      static_cast<float>(config.screenWidth) / 2 - 100,
                      pvpLocalGameButton.bounds.y - 50, 200, 50
                  }, "PVE"),
    singleGameButton({
                         static_cast<float>(config.screenWidth) / 2 - 100,
                         pveGameButton.bounds.y - 50, 200, 50
                     }, "Single")
{
    InitWindow(config.screenWidth, config.screenHeight, "Snake X Game of Life");
}

void Menu::update()
{
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
                 config.screenWidth / 2 - MeasureText("Snake X Game of Life", 50) / 2,
                 config.screenHeight / 2, 50, RED);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && singleGameButton.isClicked(GetMousePosition()))
        {
            const auto snake = new PlayerSnake(RED,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight) / 2});
            game = std::make_unique<SingleGame>(
                config, *snake);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pvpLocalGameButton.isClicked(GetMousePosition()))
        {
            const auto leftSnake = new PlayerSnake(RED,BLUE, config, Vector2{
                                                       0, static_cast<float>(config.gridHeight) / 2
                                                   });
            const auto rightSnake = new PlayerSnake(BLUE,BLACK, config, Vector2{
                                                        static_cast<float>(config.gridWidth - 1),
                                                        static_cast<float>(config.gridHeight) / 2
                                                    }, true);
            game = std::make_unique<PVPLocalGame>(config, *leftSnake, *rightSnake);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && pveGameButton.isClicked(GetMousePosition()))
        {
            const auto snake = new PlayerSnake(RED,BLUE, config, Vector2{
                                                   0, static_cast<float>(config.gridHeight) / 2
                                               });
            game = std::make_unique<PVEGame>(config, *snake);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && lanGameButton.isClicked(GetMousePosition()))
        {
            const auto snake = new PlayerSnake(RED,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight) / 2});
            game = std::make_unique<LANGame>(config, *snake);
        }
    }
}
