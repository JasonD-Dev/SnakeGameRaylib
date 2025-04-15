#include "Audio.h"
#include "Map.h"
#include "Snake.h"
#include "GameState.h"

// GameState
GameState Game;
const int max_y = Game.map.gridHeight - 1;
const int max_x = Game.map.gridWidth - 1;

bool nightmode = false;

void BeginGame() {
    Game.food = { 9, 6 };
    Game.snake.length = 2;
    Game.snake.direction = { 1, 0 };
    Game.snake.segments[0] = { 3, 6 }; 
    Game.snake.segments[1] = { 2, 6 };
    Game.audioManager.ResumeMusic();
    Game.score = 0;
    Game.readyToPlay = true;
    Game.gameOver = false;
    Game.win = false;
}

void Die() {
    if (!Game.gameOver) {
		Game.gameOver = true;
		Game.audioManager.PlayDie();
    }
}

void SpawnFood() {
    Vector2 temp;
    do {
        temp.x = (float)GetRandomValue(0, max_x);
        temp.y = (float)GetRandomValue(0, max_y);
    }
    while (Game.snake.IsSnake(temp));
    Game.food = temp;

    if (Game.snake.length >= 127) {
        Game.win = true;
        Die();
    }
}

void EatFood() {
    if (!Game.gameOver)
    {
		Game.food = { 0, 0 };
		Game.audioManager.PlayEat();
		SpawnFood();
		Game.snake.length++;
		Game.score++;
    }
}


void MoveSnake(Snake& snake) {
    if (snake.segments[0].x == Game.food.x && snake.segments[0].y == Game.food.y)
    {
        EatFood();
    }
	else if (Game.snake.IsSnake(snake.segments[0]))
	{
		Die();
	}
    for (int i = snake.length - 1; i > 0; i--) {
        snake.segments[i] = snake.segments[i - 1];
    }

    snake.segments[0].x = (snake.segments[0].x + snake.direction.x > max_x) ? 0 :
        (snake.segments[0].x + snake.direction.x < 0) ? max_x : snake.segments[0].x + snake.direction.x;

    snake.segments[0].y = (snake.segments[0].y + snake.direction.y > max_y) ? 0 :
        (snake.segments[0].y + snake.direction.y < 0) ? max_y : snake.segments[0].y + snake.direction.y;



    
}

void DrawRect(int posX, int posY, Color color) {
    DrawRectangle(posX * Game.map.cellSize, posY * Game.map.cellSize, Game.map.cellSize, Game.map.cellSize, color);
}

void DrawGrid()
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 14; j++) 
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
    if (!Game.win && Game.score >= Game.highestScore)
    {
        Game.highestScore = Game.score;
        DrawTextCenteredX("Congrats on a new High Score!", 400, textSize, nightmode ? GOLD : SKYBLUE);
    }
    if (Game.win) 
    {
        DrawTextCenteredX("YOU CHEATED DIDN'T YOU!", 120, titlesize-20, nightmode ? PINK : VIOLET);
        DrawTextCenteredX("You pressed the button I told you not to press.", 300, textSize-5, nightmode ? LIGHTGRAY : DARKBLUE);
		DrawTextCenteredX(">:(", 400, titlesize*4, nightmode ? RED : RED);
    }
    else {
        DrawTextCenteredX("YOU DIED", 120, titlesize, RED);
		DrawTextCenteredX(TextFormat("Score: %d", Game.score), 300, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
		DrawTextCenteredX(TextFormat("Highest Score: %d", Game.highestScore), 350, textSize, nightmode ? LIGHTGRAY : DARKBLUE);
		DrawTextCenteredX("Press R to Play Again", 550, textSize, nightmode ? GREEN : LIME);

    }
}

void DrawSnake(Snake& snake, Map& map) {
    for (int i = 0; i < snake.length; i++)
    {
        Vector2 seg = snake.segments[i];
        if (i == 0)
        {
            DrawRectangle(static_cast<int>(seg.x) * map.cellSize, static_cast<int>(seg.y) * map.cellSize, map.cellSize, map.cellSize, (nightmode) ? BLUE : DARKBLUE);
        }
        else {
            DrawRectangle(static_cast<int>(seg.x) * map.cellSize, static_cast<int>(seg.y) * map.cellSize, map.cellSize, map.cellSize, (nightmode) ? DARKBLUE : BLUE);
        }
    }
}

void DelayMoveSnake(Snake& snake) {
    Game.moveTimer += GetFrameTime();
    if (Game.moveTimer >= Game.moveInterval)
    {
        MoveSnake(snake);
        Game.moveTimer = 0;
    }
}

void DrawGame(Map& map, Snake& snake)
{
    if (!Game.readyToPlay) {
        DrawMainMenu();
    }
    else if (!Game.gameOver && Game.readyToPlay)
    {
        DrawGrid();
        // Food
        DrawRect(static_cast<int>(Game.food.x), static_cast<int>(Game.food.y), nightmode ? RED : RED);
        
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
    // begin 
    if (IsKeyReleased(KEY_R)) 
    {
        BeginGame();
    }
    // nightmode
    if (IsKeyPressed(KEY_F1)) {
        nightmode = !nightmode;
    }

	// Cheats
	if (IsKeyPressed(KEY_F2) && Game.readyToPlay) { Die(); }
	if (IsKeyDown(KEY_SPACE) && Game.readyToPlay) { EatFood(); }



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
        if (!Game.audioManager.IsMuted() && !Game.gameOver)
        {
            Game.audioManager.PauseMusic();
            Game.audioManager.ToggleMute();
        }
        else if (Game.audioManager.IsMuted() && !Game.gameOver)
        {
            Game.audioManager.ToggleMute();
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

Image ConvertFrom24to32(Image aImg) {
    Image img32 = GenImageColor(aImg.width, aImg.height, BLANK);
    for (int y = 0; y < aImg.height; y++) {
		for (int x = 0; x < aImg.width; x++) {
			Color pixel = GetImageColor(aImg, x, y); 
			pixel.a = 255; // Set alpha to opaque
			ImageDrawPixel(&img32, x, y, pixel);
		}
	}
    UnloadImage(aImg);
    return img32;
}

bool SetIcon() {
	Image icon = LoadImage("resources/Icon.jpg");
	if (icon.data == NULL)
	{
        return false;
	}
	icon = ConvertFrom24to32(icon);
	SetWindowIcon(icon);
	UnloadImage(icon);
    return true;
}


int main()
{   
    InitWindow(Game.map.windowWidth, Game.map.windowHeight, "Snake Game Raylib by Jason D'Souza");
    if (!SetIcon())
    {
        CloseWindow();
    }
    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        Game.audioManager.Update();
        InputManager(Game.snake);
        BeginDrawing();
        ClearBackground(nightmode ? DARKBROWN : RAYWHITE);
        DrawGame(Game.map, Game.snake);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}