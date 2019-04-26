#include "board.h"

Board::Board(SDL_Renderer* renderer) : Entity(renderer) {
	SDL_Surface* surface = IMG_Load("bricks.png");
	bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = IMG_Load("side.png");
	sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	srand(time(0));

	x = 16;
	y = 32;
	width = 768;
	height = 600;

}

Board::~Board() {
	SDL_DestroyTexture(bricktexture);
	SDL_DestroyTexture(sidetexture);
}

void Board::Update(float delta) {

}

void Board::Render(float delta) {
	// Render bricks
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			Brick brick = bricks[i][j];

			// Kiem tra xem brick co ton tai
			if (!brick.state)
				continue;

			SDL_Rect srcrect;

			srcrect.x = (brick.type % 2)* BOARD_BRWIDTH;
			srcrect.y = (brick.type / 2)* BOARD_BRHEIGHT;
			srcrect.w = BOARD_BRWIDTH;
			srcrect.h = BOARD_BRHEIGHT;

			SDL_Rect dstrect;
			dstrect.x = x + i * BOARD_BRWIDTH;
			dstrect.y = y + j * BOARD_BRHEIGHT;
			dstrect.w = BOARD_BRWIDTH;
			dstrect.h = BOARD_BRHEIGHT;

			SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
		}
	}

	// Render sides
	SDL_Rect dstrect;
	dstrect.x = 0;
	dstrect.y = 32;
	dstrect.w = 16;
	dstrect.h = 600;
	SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

	dstrect.x = 800 - 16;
	dstrect.y = 32;
	dstrect.w = 16;
	dstrect.h = 600;
	SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);
}

void Board::CreateLevel() {
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			Brick brick;

			brick.type = rand() % 4;    // Random color
			brick.state = true;         // Brick ton tai
			bricks[i][j] = brick;
		}
	}
}