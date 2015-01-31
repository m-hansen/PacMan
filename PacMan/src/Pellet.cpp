#include "Pellet.h"

Pellet::Pellet(Node* node)
{
	size = G_SIZE / 4;
	location = node;

	// Set the bounding rectangle
	boundingRect.w = size;
	boundingRect.h = size;
	boundingRect.x = (G_SIZE * location->GetLocation().x) + (G_SIZE / 2 - size / 2);
	boundingRect.y = (G_SIZE * location->GetLocation().y) + (G_SIZE / 2 - size / 2);
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