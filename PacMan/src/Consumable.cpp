#include "Consumable.h"

Consumable::Consumable(ConsumableTypeEnum consumableType, float x, float y)
{
	position.x = x;
	position.y = y;
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

void Consumable::DetermineTexture()
{
	switch (type)
	{
	case ConsumableTypeEnum::ConsumablePellet:
		texture = TextureManager::GetTexture("pellet");
		break;
	case ConsumableTypeEnum::ConsumablePowerPellet:
		texture = TextureManager::GetTexture("powerPellet.png");
		break;
	case ConsumableTypeEnum::ConsumableFruit:
		// TODO 
		break;
	}

	if (texture == NULL)
		fprintf(stdout, "Comsumable texture not found!\n");
}

SDL_Rect* Consumable::GetBoundingRect()
{
	return &boundingRect;
}