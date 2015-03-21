#include "Wall.h"

Wall::Wall(Node* node)
{
	int boundingSize = Config::gridSize - 2; // makes the collision rectangle slightly smaller
	int renderingSize = Config::gridSize;

	location = node;

	// Set the bounding rectangle
	boundingRect.w = boundingSize;
	boundingRect.h = boundingSize;
	boundingRect.x = (Config::gridSize * location->GetPosition().x) + (Config::gridSize / 2 - boundingSize / 2);
	boundingRect.y = (Config::gridSize * location->GetPosition().y) + (Config::gridSize / 2 - boundingSize / 2);

	// Set the rendering rectangle
	renderRect.w = renderingSize;
	renderRect.h = renderingSize;
	renderRect.x = (Config::gridSize * location->GetPosition().x) + (Config::gridSize / 2 - renderingSize / 2);
	renderRect.y = (Config::gridSize * location->GetPosition().y) + (Config::gridSize / 2 - renderingSize / 2);
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