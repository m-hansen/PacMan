#pragma once

#include <SDL.h>
#include "TextureManager.h"
#include "Utils.h"
#include "ConsumableType.h"

class Consumable
{
public:
	Consumable(ConsumableType consumableType, float x, float y);
	~Consumable();
	void Render(SDL_Renderer* renderer);
	void HandleCollision();
	SDL_Rect* GetBoundingRect();
private:
	ConsumableType type;
	Vector2f location;
	int scoreValue;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
	void DetermineTexture();
};