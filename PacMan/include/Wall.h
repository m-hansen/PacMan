#pragma once
#include <SDL.h>
#include "Global.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Node.h"

class Wall 
{
public:
	Wall(Node* node);
	~Wall();
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect();
private:
	Node* location;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
};