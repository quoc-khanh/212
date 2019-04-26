#include "score.h"

Score::Score(SDL_Renderer* renderer) : Entity(renderer) {
	font = TTF_OpenFont("VeraMoBd.ttf", 30);
	SDL_Color white = { 255, 255, 255 };
	std::string word = "Hello World";
	SDL_Surface* scoresurface = TTF_RenderText_Solid(font, word.c_str(), white);
	SDL_Texture* scoretexture = SDL_CreateTextureFromSurface(renderer, scoresurface);
	SDL_FreeSurface (scoresurface);
}

Score::~Score()
{
	SDL_DestroyTexture(scoretexture);
}

void Score::Update(float delta) {

}

void Score::Render(float delta) {
	SDL_Rect srcRest;
	SDL_Rect desRect;
	std::string word = "Hello World";
	TTF_SizeText(font, word.c_str(), &srcRest.w, &srcRest.h);

	srcRest.x = 0;
	srcRest.y = 0;

	desRect.x = 70;
	desRect.y = 0;

	desRect.w = 100;
	desRect.h = 25;
	SDL_RenderCopy(renderer, scoretexture, &srcRest, &desRect);
	SDL_RenderPresent(renderer);
}

