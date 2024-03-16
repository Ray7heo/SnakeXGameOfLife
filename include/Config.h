#pragma once


class Config
{
public:
   Config(int gridWidth, int gridHeight, int tileSize);
   Config();
   int gridWidth;
   int gridHeight;
   int tileSize;
};
