#include "../include/Config.h"

Config::Config(int gridWidth, int gridHeight, int tileSize): gridWidth(gridWidth),
                                                                      gridHeight(gridHeight),
                                                                      tileSize(tileSize)
{
}

Config::Config()
{
    gridWidth = 50;
    gridHeight = 30;
    tileSize = 30;
}
