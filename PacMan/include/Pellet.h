#pragma once
#include <SDL.h>
#include "TextureManager.h"
#include "Node.h"

class Pellet
{
public:
	Pellet(Node* node);
	~Pellet();
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect();
private:
	Node* location;
	SDL_Rect boundingRect;
	int size;
};