#include "Audio.h"
#include "GameState.h"

// GameState
GameState Game = {
    Game.moveInterval = 0.2,
    Game.snakeLength = 2,
};
SnakeBody body;
Map map;

void DrawRect(int posX, int posY, Color color) {
    DrawRectangle(
        posX * map.cellSize,  // Convert grid position to pixel coordinates
        posY * map.cellSize,
        map.cellSize,
        map.cellSize,
        color
    );
}

void DrawGrid()
{
    for (int i = 0; i < 13; i++) // row
    {
        for (int j = 0; j < 14; j++) // colomn
        {
            if (map.grid[i][j] == ' ' && (i + j) % 2 == 0)
            {
                DrawRect(j, i, GREEN);
            }
            else if (map.grid[i][j] == ' ')
            {
                DrawRect(j, i, DARKGREEN);
            }
        }
    }
}

void DrawGame()
{
    DrawGrid();

    // Food
    DrawRect(Game.food.x, Game.food.y, RED);
    // Snake
    DrawRect(Game.snake.x, Game.snake.y, BLUE);
}

int main()
{   
    Audio gameAudio;
    InitWindow(map.windowWidth, map.windowHeight, "Snake Game Raylib by Jason D'Souza");
    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        gameAudio.UpdateMusic();
        if (IsKeyDown(KEY_A)) {
            gameAudio.PlayEatSoundEffect();
        }
        if (IsKeyDown(KEY_S)) {
            gameAudio.PlayDieSoundEffect();
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (gameAudio.IsMusicPlaying())
            {
                gameAudio.PauseMusic();
            }
            else
            {
                gameAudio.ResumeMusic();
            }
        }
        if (IsKeyPressed(KEY_UP)) {
            gameAudio.IncreaseVolume();

        }
        if (IsKeyPressed(KEY_DOWN)) {
            gameAudio.DecreaseVolume();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}