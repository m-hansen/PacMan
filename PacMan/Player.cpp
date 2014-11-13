#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	const int SIZE = 8;
	boundingRect.w = SIZE;
	boundingRect.h = SIZE;

	livesLeft = 3;
	speed = 0.1f;
	position.x = 20.0f;
	position.y = 20.0f;
}

void Player::Update(Uint32 deltaT)
{
	// Update the position
	switch (direction)
	{
		case Up:
			position.y -= speed * deltaT;
			break;
		case Down:
			position.y += speed * deltaT;
			break;
		case Left:
			position.x -= speed * deltaT;
			break;
		case Right:
			position.x += speed * deltaT;
			break;
	}

	// Update the bounding rectangle
	boundingRect.x = position.x;
	boundingRect.y = position.y;
}

void Player::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture("pacman"), NULL, &boundingRect);
}

void Player::SetDirection(DirectionEnum dirEnum)
{
	direction = dirEnum;
}

SDL_Rect* Player::GetBoundingRect()
{
	return &boundingRect;
}