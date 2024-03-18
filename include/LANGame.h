#pragma once
#include "GameBase.h"
#include "TextInput.h"


class LANGame final : public GameBase
{
public:
    LANGame();

    LANGame(const GameConfig& config, SnakeBase& snake);
    void update() override;
    void draw() override;

protected:
    void restart() override;
    TextInput textInput;

};
