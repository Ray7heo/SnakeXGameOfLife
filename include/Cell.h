#pragma once

enum CellType
{
    Edible,
    Rot,
    Die,
    Wall
};

class Cell
{
public:
    CellType type;
};
