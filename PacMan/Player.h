#pragma once
#include <SDL.h>
#include "TextureManager.h"
#include "MovingEntity.h"
#include "DirectionEnum.h"

class Player : public MovingEntity
{
public:
	Player();
	~Player();
	void Initialize();
	void Update(Uint32 deltaT);
	void Render(SDL_Renderer* renderer);
	void SetDirection(DirectionEnum dirEnum);
	SDL_Rect* GetBoundingRect();
private:
	int livesLeft;
	SDL_Rect boundingRect;
	DirectionEnum direction;
};