#pragma once
#include "GameBase.h"
#include "TextInput.h"
#include "TextInput.h"
#include <fstream>
#include <map>
#include <set>

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

    struct CompareMaps
    {
        bool operator()(const std::map<std::string, int>& a, const std::map<std::string, int>& b) const;
    };

    std::set<std::map<std::string, int>, CompareMaps> scores;
    void loadFile();
};
