#include "Wall.h"

Wall::Wall(float x, float y)
{
	location.x = x;
	location.y = y;
	texture = TextureManager::GetTexture("wall");
}

Wall::~Wall()
{
}

void Wall::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}