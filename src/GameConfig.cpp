#include "../include/GameConfig.h"

GameConfig::GameConfig(int gridWidth, int gridHeight, int tileSize): gridWidth(gridWidth),
                                                                     gridHeight(gridHeight),
                                                                     tileSize(tileSize), screenWidth(gridWidth * tileSize), screenHeight(gridHeight * tileSize)
{
}

GameConfig::GameConfig(int gridWidth, int gridHeight, int screenWidth, int screenHeight): gridWidth(gridWidth),
    gridHeight(gridHeight), screenWidth(screenWidth),
    screenHeight(screenHeight),
    tileSize(screenWidth / gridWidth)
{
}

GameConfig::GameConfig()
{
    gridWidth = 50;
    gridHeight = 30;
    tileSize = 30;
}
