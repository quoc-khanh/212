#include "ball.h"

Ball::Ball(SDL_Renderer* renderer) : Entity(renderer) {
	SDL_Surface* surface = IMG_Load("ball.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	width = 24;
	height = 24;

	SetDirection(1, 1);
}

Ball::~Ball() {
	SDL_DestroyTexture(texture);
}

void Ball::Update(float delta) {
	x += dirx * delta;
	y += diry * delta;
}

void Ball::Render(float delta) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_RenderCopy(renderer, texture, 0, &rect);
}

void Ball::SetDirection(float dirx, float diry) {
	float length = sqrt(dirx * dirx + diry * diry);
	this->dirx = BALL_SPEED * (dirx / length);
	this->diry = BALL_SPEED * (diry / length);
}