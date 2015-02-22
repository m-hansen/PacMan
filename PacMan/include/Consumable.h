#pragma once

#include <SDL.h>
#include "TextureManager.h"
#include "Utils.h"
#include "ConsumableTypeEnum.h"
#include "Node.h"

class Consumable
{
public:
	Consumable(ConsumableTypeEnum consumableType, float x, float y);
	~Consumable();
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect();
private:
	ConsumableTypeEnum type;
	Vector2f position;
	int scoreValue;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
	void DetermineTexture();
};