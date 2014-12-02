#include "Wall.h"

Wall::Wall(Node* node)
{
	int size = 8;
	location = node;

	// Set the bounding rectangle
	const int GRID_SIZE = 8;
	boundingRect.w = size;
	boundingRect.h = size;
	boundingRect.x = (GRID_SIZE * location->GetLocation().x) + (GRID_SIZE / 2 - size / 2);
	boundingRect.y = (GRID_SIZE * location->GetLocation().y) + (GRID_SIZE / 2 - size / 2);
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