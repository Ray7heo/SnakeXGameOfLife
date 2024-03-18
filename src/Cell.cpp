#include "../include/Cell.h"

#include <locale>
#include <thread>

WeightedCell::WeightedCell(const CellType type, const int weight): type(type), weight(weight)
{
}

Cell::Cell(const GameConfig& config): type(CellType::Blank), position(), config(config)
{
}

Cell::Cell(const CellType type, const Vector2& position, const GameConfig& config): type(type),
    position(position),
    config(config)
{
    // std::thread([this]()
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    //     this->type = CellType::Rot;
    // }).detach();
}


void Cell::draw() const
{
    Color color = WHITE;
    switch (type)
    {
    case CellType::Blank:
        return;
    case CellType::Edible:
        color = GREEN;
        break;
    case CellType::Rot:
        color = DARKGREEN;
        break;
    case CellType::Die:
        color = RED;
        break;
    case CellType::Wall:
        color = BLACK;
        break;
    }
    DrawRectangle(position.x * config.tileSize, position.y * config.tileSize, config.tileSize, config.tileSize, color);
}

CellType Cell::randomType(const std::vector<WeightedCell>& weightedCells)
{
    int totalWeight = 0;
    for (const auto& cell : weightedCells)
    {
        totalWeight += cell.weight;
    }
    // 生成随机数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, totalWeight);
    const int randomNumber = dist(gen);

    int cumulativeWeight = 0;
    for (const auto& cell : weightedCells)
    {
        cumulativeWeight += cell.weight;
        if (randomNumber <= cumulativeWeight)
        {
            return cell.type;
        }
    }
    return CellType::Blank;
}

void Cell::reductionCounter()
{
    if (counter <= 0) return;
    counter--;
}

void Cell::resurgence()
{
    if (counter > 0)
    {
        type = CellType::Edible;
    }
    else
    {
        type = CellType::Rot;
    }
}
