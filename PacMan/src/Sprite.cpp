#include "Sprite.h"

Sprite::Sprite()
{
	texture = NULL;
	boundingRect = NULL;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
}