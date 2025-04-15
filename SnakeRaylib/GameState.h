#pragma once

struct GameState {
    Map map;
    Snake snake;
    Audio audioManager;

    bool readyToPlay = false;
    bool gameOver = false;
    Vector2 food;
    float moveTimer;
    float moveInterval = 0.15f;
    short int score = 0;
    short int highestScore = 0;
    bool win = false;
};
