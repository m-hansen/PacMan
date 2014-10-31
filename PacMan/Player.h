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
	void Update();
	void Render(SDL_Renderer* renderer);
	void SetDirection(DirectionEnum dirEnum);
private:
	int livesLeft;
	SDL_Rect boundingRect;
	DirectionEnum direction;
};