#pragma once
#include "GameBase.h"
#include "TextInput.h"
#include "TextInput.h"

class SingleGame final : public GameBase
{
public:
    SingleGame();
    SingleGame(const GameConfig& config, SnakeBase& snake);

    void update() override;
    void draw() override;

protected:
    void restart() override;
private:
    TextInput textInput;
};
