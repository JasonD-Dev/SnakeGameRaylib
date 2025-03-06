#include "raylib.h"

const int cellSize = 20;
const int gridWidth = 25;
const int gridHeight = 25;
const int windowWidth = gridWidth * cellSize;
const int windowHeight = gridHeight * cellSize;


int main()
{
    InitWindow(800, 450, "raylib - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}