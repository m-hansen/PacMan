#include "Player.h"

Player::Player()
{
	currentNode = NULL;
	previousNode = NULL;
	isAlignedWithTile = false;
	texture = TextureManager::GetTexture("pacmanRight");
	isAlive = true;
	isMoving = true;
	isVisible = true;
	queuedDirection = DirectionEnum::None;
	boundingRect.w = 1;
	boundingRect.h = 1;
	spriteRect.w = Config::gridSize;
	spriteRect.h = Config::gridSize;
	speed = 0.05f * (Config::gridSize / 8); // adjust speed based on grid size
}

Player::~Player()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;
}

void Player::ResetPosition()
{
	fprintf(stdout, "Resetting player position to spawn point\n");

	// Set the spawn point
	const Vector2f SPAWN_POINT = { 112.0f, 188.0f };
	isAlignedWithTile = false;

	// Reset the position
	position.x = SPAWN_POINT.x * (Config::gridSize / 8);
	position.y = SPAWN_POINT.y * (Config::gridSize / 8);

	// Reset the collider position
	boundingRect.x = position.x;
	boundingRect.y = position.y;

	// Reset the sprite rectangle position
	spriteRect.x = position.x - (Config::gridSize / 2);
	spriteRect.y = position.y - (Config::gridSize / 2);

	// Set the default direction
	direction = DirectionEnum::Right;
	previousDirection = direction;

	// Clear the queued direction
	queuedDirection = DirectionEnum::None;
}

void Player::Update(Uint32 deltaT)
{
	// Do not update is player has run out of lives
	if (!isAlive) return;

	// Check to see if we are centered at a node
	if (currentNode != NULL)
	{
		// Account for rounding errors
		float posX = (position.x - (currentNode->GetPosition().x)) - (Config::gridSize / 2);
		float posY = (position.y - (currentNode->GetPosition().y)) - (Config::gridSize / 2);
		if (posX < 0) posX *= -1;
		if (posY < 0) posY *= -1;

		// Check if player is aligned within the grid
		if (posX < (Config::gridSize / 2) + Config::gridSize - 1 && posY < (Config::gridSize / 2) + Config::gridSize - 1)
		{
			isAlignedWithTile = true;
		}
		else
		{
			isAlignedWithTile = false;
		}

		// Check if player is centered in a tile
		if (posX < 1 && posY < 1)
		{
			isCenteredOnTile = true;
		}
		else
		{
			isCenteredOnTile = false;
		}
	}

	if ((isCenteredOnTile) && (queuedDirection != DirectionEnum::None))
	{
		ConsumeQueuedMovement();
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
	spriteRect.x = position.x - (Config::gridSize / 2);
	spriteRect.y = position.y - (Config::gridSize / 2);
}

void Player::UpdateNodes(Node* newNode)
{
	previousNode = currentNode;
	currentNode = newNode;
}

void Player::Render(SDL_Renderer* renderer)
{
	// Don't render if object is not visible
	if (!isVisible) return;

	SDL_RenderCopy(renderer, texture, NULL, &spriteRect);
}

void Player::Kill()
{
	isAlive = false;
	isVisible = false;
}

void Player::ConsumeQueuedMovement()
{
	bool readyToConsume = false;

	// First check neighboring nodes
	std::vector<Node*> neighbors = currentNode->GetNeighborNodes();
	for (std::vector<Node*>::iterator iter = neighbors.begin();
		iter != neighbors.end(); ++iter)
	{
		switch (queuedDirection)
		{
		case Up:
			if ((*iter)->GetNodeId() < (currentNode->GetNodeId() - 1))
				readyToConsume = true;
			break;
		case Down:
			if ((*iter)->GetNodeId() > (currentNode->GetNodeId() + 1))
				readyToConsume = true;
			break;
		case Left:
			if ((*iter)->GetNodeId() == (currentNode->GetNodeId() - 1))
				readyToConsume = true;
			break;
		case Right:
			// break out if we cannot yet move in the queued direction
			if ((*iter)->GetNodeId() == (currentNode->GetNodeId() + 1))
				readyToConsume = true;
			break;
		case None:
			break;
		}
	}

	// break out early if we cannot yet move in the queued direction
	if (!readyToConsume)
		return;

	position.x = (currentNode->GetPosition().x) + (Config::gridSize / 2);
	position.y = (currentNode->GetPosition().y) + (Config::gridSize / 2);
	direction = queuedDirection;
	queuedDirection = DirectionEnum::None;
}

void Player::SetDirection(DirectionEnum dirEnum)
{
	previousDirection = direction;
	direction = dirEnum;
}

void Player::QueueDirection(DirectionEnum dirEnum)
{
	queuedDirection = dirEnum;
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
	position.x = (node->GetPosition().x) + (Config::gridSize / 2);
	position.y = (node->GetPosition().y) + (Config::gridSize / 2);
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