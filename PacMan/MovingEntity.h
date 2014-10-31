#pragma once

struct Vector2f
{
	float x;
	float y;
};

class MovingEntity
{
public:
	MovingEntity();
	~MovingEntity();
protected:
	Vector2f position;
	float speed;
};