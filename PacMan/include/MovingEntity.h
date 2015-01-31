#pragma once
#include "Utils.h"

class MovingEntity
{
public:
	MovingEntity();
	~MovingEntity();
protected:
	Vector2f position;
	float speed;
};