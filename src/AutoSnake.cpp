#include "../include/AutoSnake.h"

#include "../include/AStart.h"

AutoSnake::AutoSnake(const Color& color, const Config& config): SnakeBase(color, config),
                                                                aStar(config.screenWidth, config.screenHeight)
{
}

void AutoSnake::move()
{
}

void AutoSnake::autoMove(Vector2& foodPosition)
{
        
}
