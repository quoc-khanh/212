#ifndef  SCORE_H_
#define SCORE_H_

#include "entity.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
using namespace std;
class Score : public Entity
{
public:
	Score(SDL_Renderer* renderer);
	~Score();

	void Update (float denta);
	void Render (float denta);
	
private:
	SDL_Texture* scoretexture;
	TTF_Font* font = NULL;
	string word = "Hello World";
};

#endif // ! SCORE_H_
