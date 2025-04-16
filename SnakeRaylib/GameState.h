#pragma once
#include "Audio.h"
#include "Snake.h"
#include "Map.h"

class GameState
{
public:
	Map map;
	Snake snake;
	Audio audioManager;
	void InputManager(Snake& snake);
	void DelayUpdateSnake(Snake& snake);
	bool GetNightMode() const { return nightmode; }
	Vector2 GetFood() const { return food; }
	bool GetReadyToPlay() const { return readyToPlay; }
	bool GetGameOver() const { return gameOver; }
	short int GetScore() const { return score; }
	short int GetHighestScore() const { return highestScore; }
	void SetHighestScore(short int score) { highestScore = score; }
	bool GetWin() const { return win; }

private:
	bool readyToPlay = false;
	bool gameOver = false;
	Vector2 food;
	float moveTimer = 0.0f;
	float moveInterval = 0.15f;
	float commandTimer = 0.0f;
	float commandInterval = 0.1f;
	short int score = 0;
	short int highestScore = 0;
	bool win = false;
	bool nightmode = false;
	const int max_y = map.gridHeight - 1;
	const int max_x = map.gridWidth - 1;

	void BeginGame();
	void Die();
	void SpawnFood();
	void EatFood();
	void CheatFood();
	void UpdateSnake(Snake& snake);
	void MoveCommand(float x, float y, Snake& snake);
};