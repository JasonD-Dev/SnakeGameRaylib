#pragma once
#include "raylib.h"
#include "GameState.h"

class GameRenderer
{
public:
	void RunGame();

private:
	GameState Game;
	int titlesize = 56;
	int textSize = 30;

	void DrawRect(int posX, int posY, Color color);
	void DrawGrid(Map& map);
	void DrawTextCenteredX(const char* msg, int y, int fontSize, Color col);
	void DrawMainMenu();
	void DrawGameOverScreen(GameState& Game);
	void DrawSnake(Snake& snake, Map& map);
	void DrawGame(Map& map, Snake& snake, GameState& Game);
	Image ConvertFrom24to32(Image aImg);
	bool SetIcon();

};