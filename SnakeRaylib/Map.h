#pragma once

class Map {
public:
    const int cellSize = 55;
    const int gridWidth = 14;
    const int gridHeight = 13;
    const int windowWidth = gridWidth * cellSize;
    const int windowHeight = gridHeight * cellSize;
    char grid[13][14];
    Map();
};