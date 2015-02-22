#include "Wall.h"

Wall::Wall(Node* node)
{
	int boundingSize = GRID_SIZE - 2; // makes the collision rectangle slightly smaller
	int renderingSize = GRID_SIZE;

	location = node;

	// Set the bounding rectangle
	boundingRect.w = boundingSize;
	boundingRect.h = boundingSize;
	boundingRect.x = (GRID_SIZE * location->GetPosition().x) + (GRID_SIZE / 2 - boundingSize / 2);
	boundingRect.y = (GRID_SIZE * location->GetPosition().y) + (GRID_SIZE / 2 - boundingSize / 2);

	// Set the rendering rectangle
	renderRect.w = renderingSize;
	renderRect.h = renderingSize;
	renderRect.x = (GRID_SIZE * location->GetPosition().x) + (GRID_SIZE / 2 - renderingSize / 2);
	renderRect.y = (GRID_SIZE * location->GetPosition().y) + (GRID_SIZE / 2 - renderingSize / 2);
}

Wall::~Wall()
{
}

void Wall::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture("wall"), NULL, &renderRect);
}

SDL_Rect* Wall::GetBoundingRect()
{
	return &boundingRect;
}