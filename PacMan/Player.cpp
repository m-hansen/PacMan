#include "Player.h"

Player::Player()
{
	currentNode = NULL;
	previousNode = NULL;
	isCenteredOnTile = false;
	texture = TextureManager::GetTexture("pacman");
}

Player::~Player()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;
}

void Player::Initialize()
{
	isCenteredOnTile = false;
	spriteRect.w = G_SIZE;
	spriteRect.h = G_SIZE;
	boundingRect.w = 1;
	boundingRect.h = 1;

	livesLeft = 3;
	speed = 0.05f * (G_SIZE / 8); // adjust speed based on grid size
	position.x = 112.0f * (G_SIZE / 8);
	position.y = 188.0f * (G_SIZE / 8);

	direction = DirectionEnum::Right;
	previousDirection = direction;
}

void Player::Update(Uint32 deltaT)
{
	// Check to see if we are centered at a node
	if (currentNode != NULL)
	{
		// Account for rounding errors
		float posX = position.x - (currentNode->GetLocation().x * G_SIZE);
		float posY = position.y - (currentNode->GetLocation().y * G_SIZE);
		if (posX < 0) posX *= -1;
		if (posY < 0) posY *= -1;

		if (posX < (G_SIZE / 2) + G_SIZE - 1 && posY < (G_SIZE / 2) + G_SIZE - 1)
		{
			isCenteredOnTile = true;
		}
		else
		{
			isCenteredOnTile = false;
		}
	}

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
		case None:
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
	SDL_RenderCopy(renderer, texture, NULL, &spriteRect);
}

void Player::SetDirection(DirectionEnum dirEnum)
{
	previousDirection = direction;
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

void Player::SetPosition(Node* node)
{
	position.x = (G_SIZE * node->GetLocation().x) + (G_SIZE / 2);
	position.y = (G_SIZE * node->GetLocation().y) + (G_SIZE / 2);
	currentNode = node;
}

DirectionEnum Player::GetDirection()
{
	return direction;
}

DirectionEnum Player::GetPreviousDirection()
{
	return previousDirection;
}

void Player::SetPreviousDirection(DirectionEnum dir)
{
	previousDirection = dir;
}