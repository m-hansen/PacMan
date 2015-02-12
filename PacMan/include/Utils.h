#pragma once
#include "SDL.h"

struct Vector2
{
	int x;
	int y;
};

struct Vector2f
{
	float x;
	float y;
};

class Utils
{
public:
	static bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
private:
	Utils();
};