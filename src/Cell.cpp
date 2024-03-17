#include "../include/Cell.h"

#include <locale>
#include <thread>

WeightedCell::WeightedCell(const CellType type, const int weight): type(type), weight(weight)
{
}

Cell::Cell(const GameConfig& config): type(CellType::Blank), position(), config(config)
{
}

Cell::Cell(CellType type, const Vector2& position, const GameConfig& config): type(type),
                                                                              position(position),
                                                                              config(config)
{
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
