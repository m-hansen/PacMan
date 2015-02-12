#pragma once
#include "SDL.h"
#include "Global.h"

class IScreen
{
public:
	IScreen(SDL_Renderer* r) { renderer = r; };
	virtual ~IScreen() {};
	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	SDL_Renderer* renderer;
};