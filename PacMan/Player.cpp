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
	speed = 2.0f;
	position.x = 20.0f;
	position.y = 20.0f;
}

void Player::Update()
{
	boundingRect.x = position.x;
	boundingRect.y = position.y;
	
	position.x += 0.1;
}

void Player::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture("pacman"), NULL, &boundingRect);
}