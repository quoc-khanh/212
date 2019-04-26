#include "game.h"

Game::Game() {
	window = 0;
	renderer = 0;
}

Game::~Game() {

}

bool Game::Init() {

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Brick Break - Nguyen Quoc Khanh",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "Error creating window:" << SDL_GetError() << endl;
		return false;
	}


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Error creating renderer:" << SDL_GetError() << endl;
		return false;
	}

	if (TTF_Init() < 0) {
		cout << "TTF could not initialize!" << SDL_GetError() << endl;
		return false;
	}

	
	lasttick = SDL_GetTicks();
	fpstick = lasttick;
	fps = 0;
	framecount = 0;

	return true;
}

void Game::Clean() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Game::Run() {
	board = new Board(renderer);
	paddle = new Paddle(renderer);
	ball = new Ball(renderer);

	BeginMenu();

	NewGame();

	while (true) {
		
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		// Tinh toan delta and fps
		unsigned int curtick = SDL_GetTicks();
		float delta = (curtick - lasttick) / 1000.0f;
		//cout << "Fpstick: " << fpstick << " " << "Curtick: " << curtick << " " << "FPS: " << fps << endl;
		if (curtick - fpstick >= FPS_DELAY) {
			fps = framecount * (1000.0f / (curtick - fpstick));
			fpstick = curtick;
			framecount = 0;
			char buf[100];
			snprintf(buf, 100, "Nguyen Quoc Khanh - Brick Break (fps: %u)", fps);
			SDL_SetWindowTitle(window, buf);
		}
		else {
			framecount++;
		}
		lasttick = curtick;

		Update(delta);
		Render(delta);
	}

	delete board;
	delete paddle;
	delete ball;

	Clean();

	TTF_Quit();
	SDL_Quit();
}

void Game::BeginMenu() {
	tempSurface = IMG_Load("start.png");
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	bool isRunning = true;

	while (isRunning)
	{
		SDL_RenderClear(renderer);
		SDL_Event g_event;
		while (SDL_PollEvent(&g_event)) {
			if (g_event.type == SDL_QUIT) {
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				break;
			}
			else if (g_event.key.keysym.sym == SDLK_RETURN) {
				isRunning = false;
				break;
			}
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
	}
}

void Game::NewGame() {
	board->CreateLevel();
	ResetPaddle();
}

void Game::ResetPaddle() {
	paddlestick = true;
	StickBall();
}

void Game::StickBall() {
	ball->x = paddle->x + paddle->width / 2 - ball->width / 2;
	ball->y = paddle->y - ball->height;
}

void Game::LoadScore()
{
	int BrickLeft = GetBrickCount();
	int m = 144 - BrickLeft;
	int a;
	SDL_Surface* scoreSurface = NULL;
	SDL_Texture* scoreTexture = NULL;
	SDL_Surface* valueSurface = NULL;
	SDL_Texture* valueTexture = NULL;
	TTF_Font* font = NULL;


	font = TTF_OpenFont("VeraMoBd.ttf", 30);
	if (font == NULL)
	{
		cout << "Error load font: " << SDL_GetError() << endl;
	}
	SDL_Color White = { 255, 255, 255 };
	std::string text = "SCORE: ";

	scoreSurface = TTF_RenderText_Solid(font, text.c_str(), White);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);
	
	valueSurface = TTF_RenderText_Solid(font, (to_string(m)).c_str(), White);
	valueTexture = SDL_CreateTextureFromSurface(renderer, valueSurface);
	SDL_FreeSurface(valueSurface);

	SDL_Rect scorerect;
	scorerect.x = 0;
	scorerect.y = 0;
	scorerect.w = 100;
	scorerect.h = 30;

	SDL_RenderCopy(renderer, scoreTexture, NULL, &scorerect);

	if (m < 10) {
		a = 1;
	}
	if (m >= 10 && m <= 99) {
		a = 2;
	}
	if (m >= 100) {
		a = 3;
	}
	SDL_Rect valuerect;
	valuerect.x = 105;
	valuerect.y = 0;
	valuerect.w = 12*a;
	valuerect.h = 30;
	SDL_RenderCopy(renderer, valueTexture, NULL, &valuerect);

	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(valueTexture);
	TTF_CloseFont(font);

}

void Game::Update(float delta) {
	// Game logic

	// Input
	int mx, my;
	Uint8 mstate = SDL_GetMouseState(&mx, &my);
	SetPaddleX(mx - paddle->width / 2.0f);

	if (paddlestick) {
		StickBall();
	}
	
	if (mstate&SDL_BUTTON(1)) {
		if (paddlestick) {
			paddlestick = false;
			ball->SetDirection(1, -1);
		}
	}

	CheckBoardCollisions();
	CheckPaddleCollisions();
	CheckBrickCollisions();

	if (GetBrickCount() == 0) {
		GameOver();
		BeginMenu();
		NewGame();
	}

	board->Update(delta);
	paddle->Update(delta);

	if (!paddlestick) {
		ball->Update(delta);
	}
}

void Game::SetPaddleX(float x) {
	float newx;
	if (x < board->x) {
		// Chan trai
		newx = board->x;
	}
	else if (x + paddle->width > board->x + board->width) {
		// Chan phai
		newx = board->x + board->width - paddle->width;
	}
	else {
		newx = x;
	}
	paddle->x = newx;
}

void Game::CheckBoardCollisions() {
	// Top and bottom collisions
	if (ball->y < board->y) {
		// Top

		ball->y = board->y;
		ball->diry *= -1;
	}
	else if (ball->y + ball->height > board->y + board->height) {
		// Bottom

		GameOver();
		BeginMenu();
		NewGame();
	}

	// Left and right collisions
	if (ball->x < board->x) {
		// Left

		ball->x = board->x;
		ball->dirx *= -1;
	}
	else if (ball->x + ball->width > board->x + board->width) {
		// Right
		
		ball->x = board->x + board->width - ball->width;
		ball->dirx *= -1;
	}
}

float Game::GetReflection(float hitx) {
	
	if (hitx < 0) {
		hitx = 0;
	}
	else if (hitx > paddle->width) {
		hitx = paddle->width;
	}
	
	hitx -= paddle->width / 2.0f;
	
	return 1.0f * (hitx / (paddle->width / 2.0f));
}


void Game::CheckPaddleCollisions() {
	float ballcenterx = ball->x + ball->width / 2.0f;

	// Kiem tra va cham voi paddle
	if (ball->Collides(paddle)) {
		ball->y = paddle->y - ball->height;
		ball->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
		
	}
}

void Game::CheckBrickCollisions() {
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			Brick brick = board->bricks[i][j];

			// Kiem tra brick con hay mat
			if (brick.state) {
				float brickx = board->x + i * BOARD_BRWIDTH;
				float bricky = board->y + j * BOARD_BRHEIGHT;

		
				float ballcenterx = ball->x + 0.5f*ball->width;
				float ballcentery = ball->y + 0.5f*ball->height;

				
				float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
				float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

				if (ball->x <= brickx + BOARD_BRWIDTH && ball->x + ball->width >= brickx && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky) {
					// Phat hien va cham, xoa brick
					board->bricks[i][j].state = false;

					// Tinh ysize
					float ymin = 0;
					if (bricky > ball->y) {
						ymin = bricky;
					}
					else {
						ymin = ball->y;
					}

					float ymax = 0;
					if (bricky + BOARD_BRHEIGHT < ball->y + ball->height) {
						ymax = bricky + BOARD_BRHEIGHT;
					}
					else {
						ymax = ball->y + ball->height;
					}

					float ysize = ymax - ymin;

			        //Tinh xsize
					float xmin = 0;
					if (brickx > ball->x) {
						xmin = brickx;
					}
					else {
						xmin = ball->x;
					}

					float xmax = 0;
					if (brickx + BOARD_BRWIDTH < ball->x + ball->width) {
						xmax = brickx + BOARD_BRWIDTH;
					}
					else {
						xmax = ball->x + ball->width;
					}

					float xsize = xmax - xmin;

					// Thiet lap collision response
					if (xsize > ysize) {
						if (ballcentery > brickcentery) {
							// Bottom
							ball->y += ysize + 0.01f; // Thoat khoi va cham
							BallBrickResponse(3);
						}
						else {
							// Top
							ball->y -= ysize + 0.01f; 
							BallBrickResponse(1);
						}
					}
					else {
						if (ballcenterx < brickcenterx) {
							// Left
							ball->x -= xsize + 0.01f; 
							BallBrickResponse(0);
						}
						else {
							// Right
							ball->x += xsize + 0.01f; 
							BallBrickResponse(2);
						}
					}

					return;
				}
			}
		}
	}
}

void Game::BallBrickResponse(int dirindex) {
	// dirindex 0: Left, 1: Top, 2: Right, 3: Bottom

	// Direction factors
	int mulx = 1;
	int muly = 1;

	if (ball->dirx > 0) {
		// Ball dang di chuyen theo chieu duong x
		if (ball->diry > 0) {
			// Ball dang di chuyen theo chieu duong y
			// +1 +1
			if (dirindex == 0) {
				mulx = -1;
			}
			else if (dirindex == 1) {
				muly = -1;
			}
		}
		else if (ball->diry < 0) {
			// Ball dang di chuyen theo chieu am y
			// +1 -1
			if (dirindex == 0) {
				mulx = -1;
			}
			else if (dirindex == 3) {
				muly = -1;
			}
		}
	}
	else if (ball->dirx < 0) {
		// Ball dang di chuyen theo chieu am x
		if (ball->diry > 0) {
			// Ball dang di chuyen theo chieu duong y
			// -1 +1
			if (dirindex == 2) {
				mulx = -1;
			}
			else if (dirindex == 1){
				muly = -1;
			}
		}
		else if (ball->diry < 0) {
			// Ball dang di chuyen theo chieu am y
			// -1 -1
			if (dirindex == 2) {
				mulx = -1;
			}
			else if (dirindex == 3) {
				muly = -1;
			}
		}
	}

	// Dinh huong lai direction cho ball
	ball->SetDirection(mulx*ball->dirx, muly*ball->diry);
}

int Game::GetBrickCount() {
	int brickcount = 0;
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			Brick brick = board->bricks[i][j];
			if (brick.state) {
				brickcount++;
			}
		}
	}

	return brickcount;
}

void Game::GameOver(){
	int BrickLeft = GetBrickCount();
	int m = 144 - BrickLeft;
	int a;
	SDL_Surface* surface = NULL;
	SDL_Texture* goTexture = NULL;
	SDL_Texture* scoreTexture = NULL;
	SDL_Texture* valueTexture = NULL;
	SDL_Texture* pressTexture = NULL;
	TTF_Font* font = NULL;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	font = TTF_OpenFont("VeraMoBd.ttf", 30);
	if (font == NULL)
	{
		cout << "Error load font: " << SDL_GetError() << endl;
	}
	SDL_Color Yellow = { 255, 255, 0, 255 };
	
	string gotext = "GAME OVER";
	surface = TTF_RenderText_Solid(font, gotext.c_str(), Yellow);
	goTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	string scoretext = "Score:";
	surface = TTF_RenderText_Solid(font, scoretext.c_str(), Yellow);
	scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Solid(font, (to_string(m)).c_str(), Yellow);
	valueTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	string presstext = "Press space to play again";
	surface = TTF_RenderText_Solid(font, presstext.c_str(), Yellow);
	pressTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect gorect;
	gorect.x = 100;
    gorect.y = 100;
	gorect.w = 550;
	gorect.h = 150;
    SDL_RenderCopy(renderer, goTexture, NULL, &gorect);

	SDL_Rect scorerect;
	scorerect.x = 130;
	scorerect.y = 250;
	scorerect.w = 200;
	scorerect.h = 100;
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scorerect);

	if (m < 10) {
		a = 1;
	}
	if (m >= 10 && m <= 99) {
		a = 2;
	}
	if (m >= 100) {
		a = 3;
	}
	SDL_Rect valuerect;
	valuerect.x = 506;
	valuerect.y = 250;
	valuerect.w = 50 * a;
	valuerect.h = 100;
	SDL_RenderCopy(renderer, valueTexture, NULL, &valuerect);

	SDL_Rect pressrect;
	pressrect.x = 100;
	pressrect.y = 400;
	pressrect.w = 600;
	pressrect.h = 100;
	SDL_RenderCopy(renderer, pressTexture, NULL, &pressrect);

	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(valueTexture);
	TTF_CloseFont(font);

	bool isRunning = true;
	while (isRunning)
	{
		SDL_RenderClear(renderer);
		SDL_Event g_event;
		while (SDL_PollEvent(&g_event)) {
			if (g_event.type == SDL_QUIT) {
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				break;
			}
			else if (g_event.key.keysym.sym == SDLK_SPACE) {
				isRunning = false;
				break;
			}
		}
	}
}

void Game::Render(float delta) {
	SDL_RenderClear(renderer);

	board->Render(delta);
	paddle->Render(delta);
	ball->Render(delta);
	LoadScore();
	SDL_RenderPresent(renderer);
}
