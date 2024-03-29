﻿#pragma once
#include "GameBase.h"

class PVEGame final : public GameBase
{
public:
    PVEGame();
    PVEGame(const GameConfig& config, SnakeBase& snake);

    void update() override;
    void draw() override;

protected:
    void restart() override;
    int countLiveNeighbors(int x, int y) const override;

private:
    std::unique_ptr<SnakeBase> autoSnake;
    int autoScore;
};
