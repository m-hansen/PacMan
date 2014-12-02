#include "Pellet.h"

Pellet::Pellet(Node* node)
{
	size = 2;
	location = node;

	// Set the bounding rectangle
	const int GRID_SIZE = 8;
	boundingRect.w = size;
	boundingRect.h = size;
	boundingRect.x = (GRID_SIZE * location->GetLocation().x) + (GRID_SIZE / 2 - size / 2);
	boundingRect.y = (GRID_SIZE * location->GetLocation().y) + (GRID_SIZE / 2 - size / 2);
}

Pellet::~Pellet()
{

}

void Pellet::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture("pellet"), NULL, &boundingRect);
}

SDL_Rect* Pellet::GetBoundingRect()
{
	return &boundingRect;
}