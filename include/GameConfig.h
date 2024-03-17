#pragma once


class GameConfig
{
public:
   GameConfig(int gridWidth, int gridHeight, int tileSize);

   GameConfig(int gridWidth, int gridHeight, int screenWidth, int screenHeight);


   GameConfig();
   int gridWidth;
   int gridHeight;
   int screenWidth;
   int screenHeight;
   int tileSize;
};
