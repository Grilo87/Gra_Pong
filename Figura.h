#pragma once
#include "raylib.h"

class Paddle {

public:
	
	Paddle(float x, float y, float h = 100, float w = 20);
	Rectangle GetRect();
	void Draw();
	void SetSpeed();

	float posX{ 0 }, posY{ 0 };
	float width{ 0 }, height{ 0 };
	float speedY{ 0 };
};

class Ball {

public:
	
	Ball();
	void SetSpeed();
	void SetPosition();
	void Draw();
	
	float speedX {0}, speedY {0};
	float posX {0}, posY {0};
	float radious {0};
};

