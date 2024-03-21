#include "../include/Menu.h"

#define RayDrawText DrawText

Menu::Menu(const GameConfig& config): headColor(RED), tailColor(BLUE),
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

        const auto fontSize = 20;
        DrawText("Head Color", 0, 0, 20, BLACK);
        DrawText("Preview Color", config.screenWidth / 2 - MeasureText("Preview Color", fontSize) / 2, 0, 20, BLACK);
        DrawText("Tail Color", config.screenWidth - MeasureText("Tail Color", fontSize), 0, 20, BLACK);

        const float tempSize = config.tileSize * 2;

        DrawRectangle(config.screenWidth / 2 - config.tileSize / 2 - tempSize, fontSize, tempSize, tempSize, headColor);
        DrawRectangle(config.screenWidth / 2 - config.tileSize / 2, fontSize, tempSize, tempSize, GREEN);
        DrawRectangle(config.screenWidth / 2 - config.tileSize / 2 + tempSize, fontSize, tempSize, tempSize, tailColor);

        // select head color
        const auto isClickedHeadRed = ClickableRectangle({0, fontSize, tempSize, tempSize}, RED).draw().isClicked(GetMousePosition());
        if (isClickedHeadRed) headColor = RED;
        const auto isClickedHeadGreen = ClickableRectangle({tempSize, fontSize, tempSize, tempSize}, GREEN).draw().isClicked(GetMousePosition());
        if (isClickedHeadGreen) headColor = GREEN;
        const auto isClickedHeadBlue = ClickableRectangle({tempSize * 2, fontSize, tempSize, tempSize}, BLUE).draw().isClicked(GetMousePosition());
        if (isClickedHeadBlue) headColor = BLUE;
        const auto isClickedHeadYellow = ClickableRectangle({tempSize * 3, fontSize, tempSize, tempSize}, YELLOW).draw().isClicked(GetMousePosition());
        if (isClickedHeadYellow) headColor = YELLOW;
        const auto isClickedHeadPurple = ClickableRectangle({tempSize * 4, fontSize, tempSize, tempSize}, PURPLE).draw().isClicked(GetMousePosition());
        if (isClickedHeadPurple) headColor = PURPLE;

        // select tail color
        const auto isClickedTailRed = ClickableRectangle({config.screenWidth - tempSize * 5, fontSize, tempSize, tempSize}, RED).draw().isClicked(GetMousePosition());
        if (isClickedTailRed) tailColor = RED;
        const auto isClickedTailGreen = ClickableRectangle({config.screenWidth - tempSize * 4, fontSize, tempSize, tempSize}, GREEN).draw().isClicked(GetMousePosition());
        if (isClickedTailGreen) tailColor = GREEN;
        const auto isClickedTailBlue = ClickableRectangle({config.screenWidth - tempSize * 3, fontSize, tempSize, tempSize}, BLUE).draw().isClicked(GetMousePosition());
        if (isClickedTailBlue) tailColor = BLUE;
        const auto isClickedTailYellow = ClickableRectangle({config.screenWidth - tempSize * 2, fontSize, tempSize, tempSize}, YELLOW).draw().isClicked(GetMousePosition());
        if (isClickedTailYellow) tailColor = YELLOW;
        const auto isClickedTailPurple = ClickableRectangle({config.screenWidth - tempSize, fontSize, tempSize, tempSize}, PURPLE).draw().isClicked(GetMousePosition());
        if (isClickedTailPurple) tailColor = PURPLE;


        if (singleGameButton.isClicked(GetMousePosition()))
        {
            const auto snake = new PlayerSnake(headColor,tailColor, config, Vector2{0, static_cast<float>(config.gridHeight) / 2});
            game = std::make_unique<SingleGame>(
                config, *snake);
        }
        if (pvpLocalGameButton.isClicked(GetMousePosition()))
        {
            const auto leftSnake = new PlayerSnake(headColor,tailColor, config, Vector2{
                                                       0, static_cast<float>(config.gridHeight) / 2
                                                   });
            const auto rightSnake = new PlayerSnake(headColor,tailColor, config, Vector2{
                                                        static_cast<float>(config.gridWidth - 1),
                                                        static_cast<float>(config.gridHeight) / 2
                                                    }, true);
            game = std::make_unique<PVPLocalGame>(config, *leftSnake, *rightSnake);
        }
        if (pveGameButton.isClicked(GetMousePosition()))
        {
            const auto snake = new PlayerSnake(headColor,tailColor, config, Vector2{
                                                   0, static_cast<float>(config.gridHeight) / 2
                                               });
            game = std::make_unique<PVEGame>(config, *snake);
        }
        if (lanGameButton.isClicked(GetMousePosition()))
        {
            const auto snake = new PlayerSnake(headColor,BLUE, config, Vector2{0, static_cast<float>(config.gridHeight) / 2});
            game = std::make_unique<LANGame>(config, *snake);
        }
    }
}
