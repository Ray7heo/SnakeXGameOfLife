#pragma once
#include "GameBase.h"

class LANGame final : public GameBase
{
public:
    LANGame();

    LANGame(const Config& config, SnakeBase& snake);
};
