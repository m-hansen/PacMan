#pragma once
#include <SDL.h>
#include "Utils.h"
#include "TextureManager.h"

class Wall 
{
public:
	Wall(float x, float y);
	~Wall();
	void Render(SDL_Renderer* renderer);
private:
	Vector2f location;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
};