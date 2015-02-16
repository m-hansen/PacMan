#include "Player.h"

Player::Player()
{
	currentNode = NULL;
	previousNode = NULL;
	isAlignedWithTile = false;
	texture = TextureManager::GetTexture("pacmanRight");
	livesTexture = TextureManager::GetTexture("pacmanRight");
	livesLeft = 3;
	isAlive = true;
	isMoving = true;
	livesLeftRect.w = G_SIZE;
	livesLeftRect.h = G_SIZE;
}

Player::~Player()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;
}

void Player::Initialize()
{
	float initialPlayerPos[2] = { 112.0f, 188.0f };

	isAlignedWithTile = false;
	spriteRect.w = G_SIZE;
	spriteRect.h = G_SIZE;
	boundingRect.w = 1;
	boundingRect.h = 1;

	speed = 0.05f * (G_SIZE / 8); // adjust speed based on grid size
	position.x = initialPlayerPos[0] * (G_SIZE / 8);
	position.y = initialPlayerPos[1] * (G_SIZE / 8);

	direction = DirectionEnum::Right;
	previousDirection = direction;
}

bool Player::Kill()
{
	livesLeft--;
	if (isAlive && livesLeft <= 0)
	{
		printf("Game Over!\n");
		isAlive = false;
		position.x = -1;
		position.y = -1;
		return true;
	}
	else
	{
		Initialize();
		return false;
	}
}

void Player::Update(Uint32 deltaT)
{
	// Do not update is player has run out of lives
	if (!isAlive) return;

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
			isAlignedWithTile = true;
		}
		else
		{
			isAlignedWithTile = false;
		}

		//// Calculate the x and y offset from a tile's origin
		//float xOffset = position.x - (currentNode->GetLocation().x * G_SIZE) + G_SIZE / 2;
		//float yOffset = position.y - (currentNode->GetLocation().y * G_SIZE) + G_SIZE / 2;
		//if (xOffset < 0) xOffset *= -1;
		//if (yOffset < 0) yOffset *= -1;

		//if ((int)xOffset == 0 && (int)yOffset == 0)
		//{
		//	isCenteredOnTile = true;
		//	printf("CENTERED!\n");
		//}
		//else
		//{
		//	isCenteredOnTile = false;
		//	printf("not!\n");
		//}
	}

	// Update the position
	if (isAlignedWithTile)
	{
		switch (direction)
		{
		case Up:
			position.y -= speed * deltaT;
			texture = TextureManager::GetTexture("pacmanUp");
			break;
		case Down:
			position.y += speed * deltaT;
			texture = TextureManager::GetTexture("pacmanDown");
			break;
		case Left:
			position.x -= speed * deltaT;
			texture = TextureManager::GetTexture("pacmanLeft");
			break;
		case Right:
			position.x += speed * deltaT;
			texture = TextureManager::GetTexture("pacmanRight");
			break;
		case None:
			break;
		}
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
	// Do not render if player has run out of lives
	if (!isAlive)return;

	SDL_RenderCopy(renderer, texture, NULL, &spriteRect);

	// Display the lives left
	for (int i = 0; i < livesLeft - 1; i++)
	{
		livesLeftRect.x = G_SIZE * i;
		livesLeftRect.y = G_SIZE * 32;
		SDL_RenderCopy(renderer, livesTexture, NULL, &livesLeftRect);
	}
	
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