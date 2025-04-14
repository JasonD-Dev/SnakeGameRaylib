#include "Audio.h"
#include "Map.h"
#include "Snake.h"
#include "GameState.h"

// GameState
GameState Game;
int max_y = 12;
int max_x = 13;
bool nightmode = false;

void BeginGame() {
    Game.food = { 9, 6 };
    Game.snake.length = 2;
    Game.snake.direction = { 1, 0 };
    Game.snake.snakeSegments[0] = { 3, 6 }; 
    Game.snake.snakeSegments[1] = { 2, 6 };
    Game.audioManager.ResumeMusic();
    Game.score = 0;
    Game.readyToPlay = true;
    Game.gameOver = false;
}


bool IsSnake(Vector2 v) {
    for (int i = 1; i < Game.snake.length; i++)
    {
        if (v.x == Game.snake.snakeSegments[i].x && v.y == Game.snake.snakeSegments[i].y) {
            return true;
        }
    }
    return false;
}

void SpawnFood() {
    Vector2 temp;
    do {
        temp.x = GetRandomValue(0, max_x);
        temp.y = GetRandomValue(0, max_y);
    }
    while (IsSnake(temp));
    Game.food = temp;

    if (Game.snake.length >= 127) {
        // No space left: player has won or game is over
        Game.win = true;
        Game.gameOver = true;
    }
}

void EatFood() {
    Game.food = { 0, 0 };
    Game.audioManager.PlayEatSoundEffect();
    SpawnFood();
    Game.score++;
    Game.snake.length++;
}

void Die() {
    Game.gameOver = true;
    Game.audioManager.PlayDieSoundEffect();
}

void MoveSnake(Snake& snake) {
    if (snake.snakeSegments[0].x == Game.food.x && snake.snakeSegments[0].y == Game.food.y)
    {
        EatFood();
    }
    else if (IsSnake(snake.snakeSegments[0]))
    {
        Die();
    }
    for (int i = snake.length - 1; i > 0; i--) {
        snake.snakeSegments[i] = snake.snakeSegments[i - 1];
    }

    snake.snakeSegments[0].x = (snake.snakeSegments[0].x + snake.direction.x > max_x) ? 0 :
        (snake.snakeSegments[0].x + snake.direction.x < 0) ? max_x : snake.snakeSegments[0].x + snake.direction.x;

    snake.snakeSegments[0].y = (snake.snakeSegments[0].y + snake.direction.y > max_y) ? 0 :
        (snake.snakeSegments[0].y + snake.direction.y < 0) ? max_y : snake.snakeSegments[0].y + snake.direction.y;



    
}

void DrawRect(int posX, int posY, Color color) {
    DrawRectangle(posX * Game.map.cellSize, posY * Game.map.cellSize, Game.map.cellSize, Game.map.cellSize, color);
}

void DrawGrid()
{
    for (int i = 0; i < 13; i++) // row
    {
        for (int j = 0; j < 14; j++) // colomn
        {
            if (Game.map.grid[i][j] == ' ' && (i + j) % 2 == 0)
            {
                DrawRect(j, i, nightmode ? DARKGRAY : BEIGE);
            }
            else if (Game.map.grid[i][j] == ' ')
            {
                DrawRect(j, i, nightmode ? BROWN : RAYWHITE);
            }
        }
    }
}

void DrawTextCenteredX(const char* msg, int y, int fontSize, Color col) {
    int textWidth = MeasureText(msg, fontSize);
    int CenteredX = (Game.map.windowWidth - textWidth) / 2;
    DrawText(msg, CenteredX, y, fontSize, col);
}

void DrawMainMenu() {
    int titlesize = 60;
    int textSize = 30;

    DrawTextCenteredX("Snake Game RayLib", 120, titlesize, nightmode ? GOLD : SKYBLUE);
    DrawTextCenteredX("Made By: Jason D'Souza", 200, 20, nightmode ? YELLOW : DARKGRAY);
    DrawTextCenteredX("WASD - move", 250, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX("M - Mute music ONLY", 300, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX("Up, Down arrow keys - Adjust Volume", 350, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX(TextFormat("Volume: %.0f", Game.audioManager.GetVolume()), 400, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX("Esc - Quit", 450, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX("Ready to Play? ", 500, textSize, nightmode ? LIGHTGRAY : MAROON);
    DrawTextCenteredX("Press R to Begin a new Game", 550, textSize, nightmode ? GREEN : LIME);
}

void DrawGameOverScreen() {
    int titlesize = 60;
    int textSize = 30;

    // Score 
    if (Game.score >= Game.highestScore)
    {
        Game.highestScore = Game.score;
        DrawTextCenteredX("Congrats on a new High Score!", 400, textSize, nightmode ? GOLD : SKYBLUE);
    }
    if (Game.win) 
    {
        DrawTextCenteredX("YOU CHEATED DIDN'T YOU!", 120, titlesize, nightmode ? PINK : VIOLET);
    }
    else {
        DrawTextCenteredX("YOU DIED", 120, titlesize, RED);
    }
    DrawTextCenteredX(TextFormat("Score: %d", Game.score), 300, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX(TextFormat("Highest Score: %d", Game.highestScore), 350, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
    DrawTextCenteredX("Press R to Play Again", 550, textSize, nightmode ? GREEN : LIME);
}

void DrawSnake(Snake& snake, Map& map) {
    for (int i = 0; i < snake.length; i++)
    {
        Vector2 seg = snake.snakeSegments[i];
        if (i == 0)
        {
            DrawRectangle(seg.x * map.cellSize, seg.y * map.cellSize, map.cellSize, map.cellSize, (nightmode) ? BLUE : BLUE);
        }
        else {
            DrawRectangle(seg.x * map.cellSize, seg.y * map.cellSize, map.cellSize, map.cellSize, (nightmode) ? DARKBLUE : DARKBLUE);
        }
    }
}

void DelayMoveSnake(Snake snake) {
    Game.moveTimer += GetFrameTime();
    if (Game.moveTimer >= Game.moveInterval)
    {
        MoveSnake(Game.snake);
        Game.moveTimer = 0;
    }
}

void DrawGame(Map map, Snake snake)
{
    if (!Game.readyToPlay) {
        DrawMainMenu();
    }
    else if (!Game.gameOver && Game.readyToPlay)
    {
        DrawGrid();
        // Food
        DrawRect(Game.food.x, Game.food.y, nightmode ? RED : RED);
        
        // Snake
        DelayMoveSnake(snake);
        DrawSnake(snake, map);

        // Score
        DrawText(TextFormat("Score: %d", Game.score), Game.map.gridWidth, 0, 30, nightmode ? RAYWHITE : DARKBLUE);
        // Volume
        DrawText(TextFormat("Volume: %.0f", Game.audioManager.GetVolume()), Game.map.gridWidth, Game.map.windowHeight-30, 30, nightmode ? RAYWHITE : DARKBLUE);
    }
    else
    {
        DrawGameOverScreen();
    }
}

void InputManager(Snake& snake) 
{

    if (IsKeyReleased(KEY_R)) 
    {
        BeginGame();
    }

    // Cheats
    if (IsKeyPressed(KEY_F1)) {
        nightmode = !nightmode;
    }
    if (IsKeyPressed(KEY_F2)) { EatFood(); }
    if (IsKeyPressed(KEY_F3)) { Die();}

    // Snake Movement
    if (IsKeyPressed(KEY_W) && Game.snake.direction.y == 0) { // North
        //std::cout << " Changing Direction" << std::endl;
        snake.direction = { 0, -1 };
    }
    if (IsKeyPressed(KEY_S) && Game.snake.direction.y == 0) { // South
        snake.direction = { 0, 1 };
    }
    if (IsKeyPressed(KEY_A) && Game.snake.direction.x == 0) { // West
        snake.direction = { -1, 0 };
    }
    if (IsKeyPressed(KEY_D) && Game.snake.direction.x == 0) { // East
        snake.direction = { 1, 0 };
    }

    // Audio Control
    if (IsKeyPressed(KEY_M)) { 
        if (!Game.audioManager.MuteMusic && !Game.gameOver)
        {
            Game.audioManager.PauseMusic();
            Game.audioManager.MuteMusic = true;
        }
        else if (Game.audioManager.MuteMusic && !Game.gameOver)
        {
            Game.audioManager.MuteMusic = false;
            Game.audioManager.ResumeMusic();
        }
    }

    if (IsKeyPressed(KEY_SPACE)) 
    {
        if (Game.audioManager.IsMusicPlaying())
        {
            Game.audioManager.PauseMusic();
        }
        else
        {
            Game.audioManager.ResumeMusic();
        }
    }

    if (IsKeyPressed(KEY_UP)) {
        Game.audioManager.IncreaseVolume();
    }

    if (IsKeyPressed(KEY_DOWN)) { 
        Game.audioManager.DecreaseVolume();
    }

}

int main()
{   
    InitWindow(Game.map.windowWidth, Game.map.windowHeight, "Snake Game Raylib by Jason D'Souza");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Game.audioManager.UpdateMusic();
        InputManager(Game.snake);
        BeginDrawing();
        ClearBackground(nightmode ? DARKBROWN : RAYWHITE);
        DrawGame(Game.map, Game.snake);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}