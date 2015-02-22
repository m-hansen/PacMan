#include "Pellet.h"

Pellet::Pellet(Node* node)
{
	
	location = node;
	type = node->GetType();
	texture = TextureManager::GetTexture("pellet");

	if (type == NodeTypeEnum::PelletNode)
	{
		value = 10;
		InitBoundingRect(GRID_SIZE / 4);
	}
	else if (type == NodeTypeEnum::PowerPelletNode)
	{
		value = 50;
		InitBoundingRect(GRID_SIZE / 2);
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
	boundingRect.x = (location->GetPosition().x) + (GRID_SIZE / 2 - size / 2);
	boundingRect.y = (location->GetPosition().y) + (GRID_SIZE / 2 - size / 2);
}

void Pellet::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);
}