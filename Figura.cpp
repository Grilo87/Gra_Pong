#include "Figura.h"
#include "raylib.h"

Paddle::Paddle(float x, float y, float h, float w) : height(h), width(w) {
		posX = x;
		posY = y;
		height = 0.2 * (float)GetScreenHeight();
		width = 0.01 * (float)GetScreenWidth();
		speedY = GetScreenHeight() * 1.2;
}
Rectangle Paddle::GetRect() {
	return Rectangle{ posX - width / 2, posY - height / 2, width, height};
}
void Paddle::SetSpeed() {
	speedY = GetScreenHeight() * 1.2;
}
void Paddle::Draw() {
	DrawRectangleRec(GetRect(), BLANK);

}
Ball::Ball() {
	radious = (0.02 * (float)GetScreenWidth() + 0.02 * (float)GetScreenHeight()) / 2;
}
void Ball::SetSpeed() {
	int x = 0, y = 0;

	while (x == 0) {
		x = GetRandomValue(-1, 1);
	}
	while (y == 0) {
		y = GetRandomValue(-1, 1);
	}
	speedX = (float)x * (float)GetScreenWidth() / 2 + (float)GetRandomValue(-200, 200);
	speedY = (float)y * (float)GetScreenWidth() / 2 + (float)GetRandomValue(-200, 200);
}
void Ball::SetPosition() {
	posX = (float)GetScreenWidth() / 2.0f;
	posY = (float)GetScreenHeight() / 2.0f;
}
void Ball::Draw() {
	DrawCircle(posX, posY, radious, BLANK);
}