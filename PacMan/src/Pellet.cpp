#include "Pellet.h"

Pellet::Pellet(Node* node)
{
	
	location = node;
	type = node->GetType();

	if (type == NodeTypeEnum::PelletNode)
	{
		value = 10;
		texture = TextureManager::GetTexture("pellet");
		InitBoundingRect(Config::gridSize / 4);
	}
	else if (type == NodeTypeEnum::PowerPelletNode)
	{
		value = 50;
		texture = TextureManager::GetTexture("powerPellet");
		InitBoundingRect(Config::gridSize / 2);
	}
	else
		fprintf(stdout, "ERROR: %s is not a valid pellet type\n", std::to_string(type));
}

Pellet::~Pellet()
{

}

void Pellet::InitBoundingRect(int size)
{
	boundingRect.w = size;
	boundingRect.h = size;
	boundingRect.x = (location->GetPosition().x) + (Config::gridSize / 2 - size / 2);
	boundingRect.y = (location->GetPosition().y) + (Config::gridSize / 2 - size / 2);
}

void Pellet::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);
}