#pragma once
#include <random>
#include <vector>

#include "GameConfig.h"
#include "../lib/RayLib/raylib.h"

enum class CellType
{
    Blank,
    Edible,
    Rot,
    Die,
    Wall
};


class WeightedCell
{
public:
    // 构造函数
    WeightedCell(CellType type, int weight);

    CellType type; // 枚举值
    int weight; // 权重
};

class Cell
{
public:
    explicit Cell(const GameConfig& config);
    Cell() = default;

    Cell(CellType type, const Vector2& position, const GameConfig& config);

    CellType type;
    Vector2 position;

    void draw() const;

    static CellType randomType(const std::vector<WeightedCell>& weightedCells);
    void reductionCounter();
    void resurgence();
private:
    GameConfig config;
    int counter = 10;

};
