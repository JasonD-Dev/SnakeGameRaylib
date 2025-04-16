#include "GameRenderer.h"

void GameRenderer::RunGame() { // Game Loop
	InitWindow(Game.map.windowWidth, Game.map.windowHeight, "Snake Game Raylib by Jason D'Souza");
	if (!SetIcon())
	{
		CloseWindow();
	}
	SetTargetFPS(120);

	while (!WindowShouldClose())
	{
		Game.audioManager.Update();
		Game.InputManager(Game.snake);
		BeginDrawing();
		ClearBackground(Game.GetNightMode() ? DARKBROWN : RAYWHITE);
		DrawGame(Game.map, Game.snake, Game);
		EndDrawing();
	}
	CloseWindow();
}

void GameRenderer::DrawRect(int posX, int posY, Color color) {
	DrawRectangle(posX * Game.map.cellSize, posY * Game.map.cellSize, Game.map.cellSize, Game.map.cellSize, color);
}

void GameRenderer::DrawGrid(Map& map)
{
	for (int i = 0; i < map.gridHeight; i++)
	{
		for (int j = 0; j < map.gridWidth; j++)
		{
			if (map.grid[i][j] == ' ' && (i + j) % 2 == 0)
			{
				DrawRect(j, i, Game.GetNightMode() ? DARKGRAY : BEIGE);
			}
			else if (map.grid[i][j] == ' ')
			{
				DrawRect(j, i, Game.GetNightMode() ? BROWN : RAYWHITE);
			}
		}
	}
}

void GameRenderer::DrawTextCenteredX(const char* msg, int y, int fontSize, Color col) {
	int textWidth = MeasureText(msg, fontSize);
	int CenteredX = (Game.map.windowWidth - textWidth) / 2;
	DrawText(msg, CenteredX, y, fontSize, col);
}

void GameRenderer::DrawMainMenu() {
	DrawTextCenteredX("Snake Game RayLib", 120, titlesize, Game.GetNightMode() ? GOLD : SKYBLUE);
	DrawTextCenteredX("Made By: Jason D'Souza", 190, 20, Game.GetNightMode() ? YELLOW : DARKGRAY);
	DrawTextCenteredX("Move : Arrow Keys or WASD", 240, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
	DrawTextCenteredX("Mute music ONLY : M", 280, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
	DrawTextCenteredX("Adjust Volume : UP (Q) / DOWN (E) ", 320, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
	DrawTextCenteredX(TextFormat("Volume : %.0f", Game.audioManager.GetVolume()), 360, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
	DrawTextCenteredX("Maximise Window : F", 400, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
	DrawTextCenteredX("Quit : ESC", 440, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
	DrawTextCenteredX("Ready to Play? ", 500, textSize, Game.GetNightMode() ? LIGHTGRAY : MAROON);
	DrawTextCenteredX("Press R to Begin a new Game", 540, textSize, Game.GetNightMode() ? GREEN : LIME);
	DrawTextCenteredX("DO NOT PRESS SPACE BAR!", 580, textSize - 10, Game.GetNightMode() ? RED : RED);
}

void GameRenderer::DrawGameOverScreen(GameState& Game) {
	int titlesize = 60;
	int textSize = 30;

	// Score 
	if (!Game.GetWin() && Game.GetScore() >= Game.GetHighestScore())
	{
		Game.SetHighestScore(Game.GetScore());
		DrawTextCenteredX("Congrats on a new High Score!", 400, textSize, Game.GetNightMode() ? GOLD : SKYBLUE);
	}
	if (Game.GetWin())
	{
		DrawTextCenteredX("YOU CHEATED DIDN'T YOU!", 120, titlesize - 20, Game.GetNightMode() ? PINK : VIOLET);
		DrawTextCenteredX("You pressed the button I told you not to press.", 300, textSize - 5, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
		DrawTextCenteredX(">:(", 400, titlesize * 4, Game.GetNightMode() ? RED : RED);
	}
	else {
		DrawTextCenteredX("YOU DIED", 120, titlesize, RED);
		DrawTextCenteredX(TextFormat("Score: %d", Game.GetScore()), 300, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
		DrawTextCenteredX(TextFormat("Highest Score: %d", Game.GetHighestScore()), 350, textSize, Game.GetNightMode() ? LIGHTGRAY : DARKBLUE);
		DrawTextCenteredX("Press R to Play Again", 550, textSize, Game.GetNightMode() ? GREEN : LIME);

	}
}

void GameRenderer::DrawSnake(Snake& snake, Map& map) {
	for (int i = 0; i < snake.length; i++)
	{
		Vector2 seg = snake.segments[i];
		if (i == 0)
		{
			DrawRectangle(static_cast<int>(seg.x) * map.cellSize, static_cast<int>(seg.y) * map.cellSize, map.cellSize, map.cellSize, (Game.GetNightMode()) ? BLUE : DARKBLUE);
		}
		else {
			DrawRectangle(static_cast<int>(seg.x) * map.cellSize, static_cast<int>(seg.y) * map.cellSize, map.cellSize, map.cellSize, (Game.GetNightMode()) ? DARKBLUE : BLUE);
		}
	}
}

void GameRenderer::DrawGame(Map& map, Snake& snake, GameState& Game)
{
	if (!Game.GetReadyToPlay()) {
		DrawMainMenu();
	}
	else if (!Game.GetGameOver() && Game.GetReadyToPlay())
	{
		DrawGrid(map);
		DrawRect(static_cast<int>(Game.GetFood().x), static_cast<int>(Game.GetFood().y), Game.GetNightMode() ? RED : RED);

		// Snake
		Game.DelayUpdateSnake(snake);
		DrawSnake(snake, map);

		// Score
		DrawText(TextFormat("Score: %d", Game.GetScore()), Game.map.gridWidth, 0, 30, Game.GetNightMode() ? RAYWHITE : DARKBLUE);
		// Volume
		DrawText(TextFormat("Volume: %.0f", Game.audioManager.GetVolume()), Game.map.gridWidth, Game.map.windowHeight - 30, 30, Game.GetNightMode() ? RAYWHITE : DARKBLUE);
	}
	else
	{
		DrawGameOverScreen(Game);
	}
}

Image GameRenderer::ConvertFrom24to32(Image aImg) {
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

bool GameRenderer::SetIcon() {
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