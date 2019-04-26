#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <SDL_ttf.h>
#include <string>


#include "board.h"
#include "paddle.h"
#include "ball.h"
//#include "score.h"
using namespace std;

#define FPS_DELAY 500

class Game {
public:
	Game();
	~Game();

	bool Init();
	void Run();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* tempSurface;
	SDL_Event mainEvent;
	SDL_Texture* texture;

	// Timing
	unsigned int lasttick, fpstick, fps, framecount;


	Board* board;
	Paddle* paddle;
	Ball* ball;
	
	bool paddlestick;

	void Clean();
	void Update(float delta);
	void Render(float delta);

	void BeginMenu();
	void NewGame();
	void LoadScore();
	void ResetPaddle();
	void StickBall();

	void SetPaddleX(float x);
	void CheckBoardCollisions();
	float GetReflection(float hitx);
	void CheckPaddleCollisions();
	void CheckBrickCollisions();
	void BallBrickResponse(int dirindex);
	int GetBrickCount();
	void GameOver();
};

#endif