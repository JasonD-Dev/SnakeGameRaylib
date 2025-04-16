#pragma once
#include "raylib.h"

class Snake {
public:
    Vector2 segments[127];
    Vector2 direction;
    int length;

    bool IsSnake(Vector2&);
};