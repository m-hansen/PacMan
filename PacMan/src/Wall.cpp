#include "Wall.h"

Wall::Wall(Node* node)
{
	int size = G_SIZE;
	location = node;

	// Set the bounding rectangle
	boundingRect.w = size;
	boundingRect.h = size;
	boundingRect.x = (G_SIZE * location->GetLocation().x) + (G_SIZE / 2 - size / 2);
	boundingRect.y = (G_SIZE * location->GetLocation().y) + (G_SIZE / 2 - size / 2);
}

Wall::~Wall()
{
}

void Wall::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture("wall"), NULL, &boundingRect);
}

SDL_Rect* Wall::GetBoundingRect()
{
	return &boundingRect;
}