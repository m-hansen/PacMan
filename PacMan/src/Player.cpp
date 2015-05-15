#include "Player.h"

Player::Player()
	: spawnPoint({ 
		112.0f * (Config::gridSize / Config::GRID_SIZE_MULTIPLIER), 
		188.0f * (Config::gridSize / Config::GRID_SIZE_MULTIPLIER)
	})
{
	currentNode = NULL;
	previousNode = NULL;
	isAlignedWithTile = false;
	//texture = TextureManager::GetTexture("pacmanRight");
	texture = TextureManager::GetTexture("player");
	isAlive = true;
	isMoving = true;
	isVisible = true;
	hasDeathAnimationFinished = false;
	queuedDirection = DirectionEnum::None;
	boundingRect.w = 1;
	boundingRect.h = 1;
	spriteRect.w = Config::gridSize;
	spriteRect.h = Config::gridSize;
	speed = 0.05f * (Config::gridSize / 8); // adjust speed based on grid size
	sprite = new Sprite(texture, spawnPoint.x, spawnPoint.y, 8, 8);
	animationFrame = 0;
	directionFrameIndex = 0;
	ClipSpriteSheets();
	animationTimer.Start();
	currentClip = &movementAnimation[animationFrame];
}

Player::~Player()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;

	delete (sprite);
	sprite = NULL;
}

// Deconstruct the sprite sheet into individual clips
void Player::ClipSpriteSheets()
{
	const int SPRITE_WIDTH = 16;
	const int SPRITE_HEIGHT = 16;

	// Set the sprite clips for the player's movement
	const int INNER_RANGE = 2;
	for (int i = 0; i < MOVEMENT_FRAMES / INNER_RANGE; i++)
	{
		for (int j = 0; j < INNER_RANGE; j++)
		{
			movementAnimation[i * INNER_RANGE + j].x = SPRITE_WIDTH * j;
			movementAnimation[i * INNER_RANGE + j].y = SPRITE_HEIGHT * i;
			movementAnimation[i * INNER_RANGE + j].w = SPRITE_WIDTH;
			movementAnimation[i * INNER_RANGE + j].h = SPRITE_HEIGHT;
		}
	}

	// Player death animation
	const int HORIZ_FRAME_OFFSET = 2;
	for (int i = 0; i < DEATH_FRAMES; i++)
	{
		deathAnimation[i].x = SPRITE_WIDTH * (i + HORIZ_FRAME_OFFSET);
		deathAnimation[i].y = 0;
		deathAnimation[i].w = SPRITE_WIDTH;
		deathAnimation[i].h = SPRITE_HEIGHT;
	}
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

	animationFrame = 0;
	directionFrameIndex = 0;
	animationTimer.Start();
	currentClip = &movementAnimation[animationFrame];

	isAlive = true;
	isVisible = true;
	hasDeathAnimationFinished = false;
}

void Player::Update(Uint32 deltaT)
{
	// Only update death animation if player has been killed
	if (!isAlive)
	{
		// Play the death animation
		currentClip = &deathAnimation[animationFrame];
		animationFrame++;
		if (animationFrame > DEATH_FRAMES)
		{
			// Death animation is over
			animationFrame = 0;
			isVisible = false;
			hasDeathAnimationFinished = true;
		}
		return;
	}

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
			//texture = TextureManager::GetTexture("pacmanUp");
			directionFrameIndex = 4;
			break;
		case Down:
			position.y += speed * deltaT;
			//texture = TextureManager::GetTexture("pacmanDown");
			directionFrameIndex = 6;
			break;
		case Left:
			position.x -= speed * deltaT;
			//texture = TextureManager::GetTexture("pacmanLeft");
			directionFrameIndex = 2;
			break;
		case Right:
			position.x += speed * deltaT;
			//texture = TextureManager::GetTexture("pacmanRight");
			directionFrameIndex = 0;
			break;
		case None:
			break;
		}
		if (direction != previousDirection)
			animationFrame = directionFrameIndex;
	}

	// Update the bounding rectangle
	boundingRect.x = position.x;
	boundingRect.y = position.y;
	spriteRect.x = position.x - (Config::gridSize / 2);
	spriteRect.y = position.y - (Config::gridSize / 2);

	// Play animations
	AnimateMovement();
}

void Player::AnimateMovement()
{
	// Update the animation frames
	const int TIME_BETWEEN_FRAMES = 60;

	if (animationTimer.GetTicks() >	TIME_BETWEEN_FRAMES)
	{
		currentClip = &movementAnimation[animationFrame];
		animationTimer.Start(); // reset the timer
		animationFrame++;
	}

	if (animationFrame > (directionFrameIndex + 1))
	{
		animationFrame = directionFrameIndex; // previously 0
	}
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

	//sprite->Render(renderer);
	SDL_RenderCopy(renderer, texture, currentClip, &spriteRect);
}

void Player::Kill()
{
	animationFrame = 0; // reset the animation frame
	isAlive = false;
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