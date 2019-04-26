#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL.h>
#include <SDL_image.h>

//khoi tao mot thuc the (toa do, va cham)

class Entity {
public:
	Entity(SDL_Renderer* renderer);
	~Entity();

	float x, y, width, height;

	void Update(float delta);
	void Render(float delta);

	bool Collides(Entity* other);
	
	SDL_Renderer* renderer;
};

#endif