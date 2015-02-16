#pragma once
#include "SDL.h"

class IRenderable
{
public:
	virtual ~IRenderable() {}
	virtual void Render(SDL_Renderer* renderer) = 0;
};