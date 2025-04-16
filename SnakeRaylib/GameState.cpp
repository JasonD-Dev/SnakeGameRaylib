#include "GameState.h"
#include <iostream>

void GameState::BeginGame() { // Transitions from different game states
	food = { 9, 6 };
	snake.length = 2;
	snake.direction = { 1, 0 };
	snake.segments[0] = { 3, 6 };
	snake.segments[1] = { 2, 6 };
	audioManager.ResumeMusic();
	score = 0;
	readyToPlay = true;
	gameOver = false;
	win = false;
}
void GameState::Die() {
	if (!gameOver) {
		gameOver = true;
		audioManager.PlayDie();
	}
}

void GameState::SpawnFood() {
	Vector2 temp;
	do {
		temp.x = (float)GetRandomValue(0, max_x);
		temp.y = (float)GetRandomValue(0, max_y);
	} while (snake.IsSnake(temp));
	food = temp;

	if (snake.length >= 127) {
		win = true;
		Die();
	}
}

void GameState::EatFood() {
	if (!gameOver)
	{
		food = { 0, 0 };
		audioManager.PlayEat();
		SpawnFood();
		snake.length++;
		score++;
	}
}

void GameState::UpdateSnake(Snake& snake) {
	if (snake.segments[0].x == food.x && snake.segments[0].y == food.y)
	{
		EatFood();
	}
	else if (snake.IsSnake(snake.segments[0]))
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

void GameState::DelayUpdateSnake(Snake& snake) {
	
	moveTimer += GetFrameTime();
	if (moveTimer >= moveInterval)
	{
		UpdateSnake(snake);
		moveTimer = 0;
	}
}

void GameState::MoveCommand(float x, float y, Snake& snake) {
	if (commandTimer >= commandInterval)
	{
		snake.direction= { x, y };
		commandTimer = 0;
	}
}

void GameState::InputManager(Snake& snake)
{
	// begin 
	if (IsKeyReleased(KEY_R))
	{
		BeginGame();
	}

	if (IsKeyReleased(KEY_F))
	{
		ToggleBorderlessWindowed();
	}

	// nightmode
	if (IsKeyPressed(KEY_F1)) {
		nightmode = !nightmode;
	}

	// Cheats
	if (IsKeyPressed(KEY_F2) && readyToPlay) { Die(); }
	if (IsKeyDown(KEY_SPACE) && readyToPlay) { EatFood(); }



	// Snake Movement
	commandTimer += GetFrameTime();
	if (IsKeyPressed(KEY_W) && snake.direction.y == 0) { // North
		MoveCommand(0, -1, snake);
	}
	if (IsKeyPressed(KEY_S) && snake.direction.y == 0) { // South
		MoveCommand(0, 1, snake);
	}
	if (IsKeyPressed(KEY_A) && snake.direction.x == 0) { // West
		MoveCommand(-1, 0, snake);
	}
	if (IsKeyPressed(KEY_D) && snake.direction.x == 0) { // East
		MoveCommand(1, 0, snake);
	}

	

	// Audio Control
	if (IsKeyPressed(KEY_M)) {
		if (!audioManager.IsMuted() && !gameOver)
		{
			audioManager.PauseMusic();
			audioManager.ToggleMute();
		}
		else if (audioManager.IsMuted() && !gameOver)
		{
			audioManager.ToggleMute();
			audioManager.ResumeMusic();
		}
	}

	if (IsKeyPressed(KEY_UP)) {
		audioManager.IncreaseVolume();
	}

	if (IsKeyPressed(KEY_DOWN)) {
		audioManager.DecreaseVolume();
	}


}