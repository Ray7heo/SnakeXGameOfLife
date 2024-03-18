#pragma once
#include "GameBase.h"

class PVPLocalGame :public GameBase
{
public:
    PVPLocalGame();

    PVPLocalGame(const GameConfig& config, SnakeBase& leftSnake, SnakeBase& rightSnake);

    void update() override;
    void draw() override;

protected:
    void restart() override;
    int countLiveNeighbors(int x, int y) const override;
private:
    std::unique_ptr<SnakeBase> rightSnake;
    int rightScore;
};
