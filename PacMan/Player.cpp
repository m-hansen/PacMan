#include "Player.h"

Player::Player()
{
	currentNode = NULL;
	previousNode = NULL;
}

Player::~Player()
{
}

void Player::Initialize()
{
	spriteRect.w = G_SIZE;
	spriteRect.h = G_SIZE;
	boundingRect.w = 1;
	boundingRect.h = 1;

	livesLeft = 3;
	speed = 0.05f * (G_SIZE / 8); // adjust speed based on grid size
	position.x = 112.0f * (G_SIZE / 8);
	position.y = 188.0f * (G_SIZE / 8);
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
	spriteRect.x = position.x - (G_SIZE / 2);
	spriteRect.y = position.y - (G_SIZE / 2);
}

void Player::UpdateNodes(Node* newNode)
{
	previousNode = currentNode;
	currentNode = newNode;
}

void Player::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture("pacman"), NULL, &spriteRect);
}

void Player::SetDirection(DirectionEnum dirEnum)
{
	direction = dirEnum;
}

SDL_Rect* Player::GetBoundingRect()
{
	return &boundingRect;
}

SDL_Rect* Player::GetSpriteRect()
{
	return &spriteRect;
}

Node* Player::GetCurrentNode()
{
	return currentNode;
}

Node* Player::GetPreviousNode()
{
	return previousNode;
}