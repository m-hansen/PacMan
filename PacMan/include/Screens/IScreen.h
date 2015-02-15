#pragma once
#include "SDL.h"
#include "Game.h"
#include "Global.h"

class IScreen
{
public:
	virtual ~IScreen() {};
	virtual void Initialize(Game* game) = 0;
	virtual void Cleanup(Game* game) = 0;
	virtual void HandleEvents(Game* game) = 0;
	virtual void Update(Game* game) = 0;
	virtual void Render(Game* game) = 0;
	void ChangeScreen(Game* game, IScreen* screen) 
	{
		game->ChangeScreen(screen);
	}
protected:
	IScreen() {}
	const int SCREEN_WIDTH = 28 * G_SIZE; // 224 width with 8x8 tiles
	const int SCREEN_HEIGHT = 36 * G_SIZE; // 288 height with 8x8 tiles
};