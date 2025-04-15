#pragma once

struct Snake {
    Vector2 segments[127];
    Vector2 direction;
    int length;

	bool IsSnake(Vector2& v) {
		for (int i = 1; i < length; i++)
		{
			if (v.x == segments[i].x && v.y == segments[i].y) {
				return true;
			}
		}
		return false;
	}
};