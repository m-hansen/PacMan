#pragma once
#include "Utils.h"

bool Utils::CollisionChecker(SDL_Rect* a, SDL_Rect* b)
{
	// Set the bounds for collider A
	int leftA = a->x;
	int rightA = a->x + a->w;
	int topA = a->y;
	int bottomA = a->y + a->h;

	// Set the bounds for collider B
	int leftB = b->x;
	int rightB = b->x + b->w;
	int topB = b->y;
	int bottomB = b->y + b->h;

	// Check for collision
	// if any of these statements are true we know that there is a gap between 
	// the two colliders on the horizontal or vertical axis
	if ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB))
	{
		return false;
	}

	// Both the horizontal and vertical coordinates have an overlap
	return true;
}