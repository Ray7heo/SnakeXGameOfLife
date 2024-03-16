#pragma once
#include "GameBase.h"

class SingleGame final : public GameBase
{
public:
    SingleGame();
    SingleGame(const Config& config, SnakeBase& snake);

    void update() override;
    void draw() override;

protected:
    void restart() override;
};
