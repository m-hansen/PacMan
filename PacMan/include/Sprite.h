#pragma once
#include <SDL.h>

class Sprite
{
public:
	Sprite();
	~Sprite();
private:
	SDL_Texture* texture;
	SDL_Rect* boundingRect;
};