#include "../include/Config.h"

Config::Config(int screen_width, int screen_height, int square_size): screenWidth(screen_width),
                                                                      screenHeight(screen_height),
                                                                      squareSize(square_size)
{
}

Config::Config()
{
    screenWidth = 800;
    screenHeight = 450;
    squareSize = 30;
}
