#include "Consumable.h"

Consumable::Consumable(ConsumableType consumableType, float x, float y)
{
	location.x = x;
	location.y = y;
	type = consumableType;
	DetermineTexture();
}

Consumable::~Consumable()
{
}

void Consumable::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);
}

void Consumable::HandleCollision()
{
	switch (type)
	{
	case ConsumableType::CPellet:
		// TODO increment the score
		break;
	case ConsumableType::CPowerPellet:
		// TODO change the state
		break;
	case ConsumableType::CFruit:
		// TODO modify the score
		break;
	}
}

void Consumable::DetermineTexture()
{
	switch (type)
	{
	case ConsumableType::CPellet:
		texture = TextureManager::GetTexture("pellet");
		break;
	case ConsumableType::CPowerPellet:
		texture = TextureManager::GetTexture("large_pellet.png");
		break;
	case ConsumableType::CFruit:
		// TODO 
		break;
	}
}

SDL_Rect* Consumable::GetBoundingRect()
{
	return &boundingRect;
}