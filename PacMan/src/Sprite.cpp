#include "Sprite.h"

Sprite::Sprite(SDL_Texture* t, float x, float y, float w, float h) : 
	texture(t), isVisible(true) 
{
	// Set the size
	boundingRect.w = w;
	boundingRect.h = h;

	// Set the position and local origin
	origin = { boundingRect.w / 2, boundingRect.h / 2 };
	boundingRect.x = x - origin.x;
	boundingRect.y = y - origin.y;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
}

void Sprite::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);
}

/*void Sprite::SetBounds(float w, float h)
{
	boundingRect.w = w;
	boundingRect.h = h;
	origin = { boundingRect.w / 2, boundingRect.h / 2 };
}*/

void Sprite::SetPosition(float x, float y)
{
	boundingRect.x = x - origin.x;
	boundingRect.y = y - origin.y;
}