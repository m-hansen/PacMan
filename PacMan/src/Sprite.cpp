#include "Sprite.h"

Sprite::Sprite()
{
	texture = NULL;
	isVisible = true;
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

void Sprite::SetBounds(float w, float h)
{
	boundingRect.w = w;
	boundingRect.h = h;
	localOrigin = { boundingRect.w / 2, boundingRect.h / 2 };
}

void Sprite::SetPosition(float x, float y)
{
	boundingRect.x = x - localOrigin.x;
	boundingRect.y = y - localOrigin.y;
}