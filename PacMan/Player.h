#pragma once
#include <SDL.h>
#include "TextureManager.h"
#include "MovingEntity.h"

class Player : public MovingEntity
{
public:
	Player();
	~Player();
	void Initialize();
	void Update();
	void Render(SDL_Renderer* renderer);
private:
	int livesLeft;
	SDL_Rect boundingRect;
};