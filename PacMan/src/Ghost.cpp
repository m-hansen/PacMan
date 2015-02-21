#include "Ghost.h"

Ghost::Ghost(std::string textureName, float spawnX, float spawnY, DirectionEnum dir)
{
	fprintf(stdout, "Creating a new ghost\n");

	texture = TextureManager::GetTexture(textureName.c_str());
	position.x = (spawnX * G_SIZE);
	position.y = (spawnY * G_SIZE);

	boundingRect.w = G_SIZE;
	boundingRect.h = G_SIZE;
	boundingRect.x = position.x;
	boundingRect.y = position.y;

	speed = 0.05f * (G_SIZE / 8);
	direction = dir;
	newDirection = DirectionEnum::None;
	previousDirection = direction;

	currentNode = NULL;
	previousNode = NULL;
	previousFrameNode = NULL;
}

Ghost::~Ghost()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;
}

void Ghost::Update(Uint32 deltaT)
{
	// Check if the AI is centered
	if (currentNode != NULL)
	{
		// Account for rounding errors
		float posX = (position.x - (currentNode->GetLocation().x * G_SIZE));
		float posY = (position.y - (currentNode->GetLocation().y * G_SIZE));
		if (posX < 0) posX *= -1;
		if (posY < 0) posY *= -1;

		if (posX < 1 && posY < 1)
		{
			isCenteredOnTile = true;
		}
		else
		{
			isCenteredOnTile = false;
		}
	}

	// Check if the ghost has changed nodes
	if (currentNode != previousFrameNode)
	{
		// Set out previous node to the node we were in last frame
		previousNode = previousFrameNode;
	}

	// Check for crossroads
	// only consider crossroads that have more than two neighbors
	// since the ghosts can only move in one direction we must always move forward
	if ((currentNode != previousFrameNode) &&
		(currentNode->GetNeighborNodes().size() > 1))
	{
		const int MAX_NEIGHBORS = 4;
		int nodeIdArray[MAX_NEIGHBORS] = {};
		int index = 0;

		// Iterate over all neighboring nodes
		std::vector<Node*> neighbors = currentNode->GetNeighborNodes();
		for (std::vector<Node*>::iterator iter = neighbors.begin(); 
			iter != neighbors.end(); ++iter)
		{
			// prevent the program from crashing if the definition of neighbors changes
			// (ie: if we decide to implement diagonal movement in the future)
			if (index >= MAX_NEIGHBORS)
			{
				printf("Breaking out of ghost node neighbor early. This could cause unintended runtime results with the AI's behavior.\n" \
					"Try updating the MAX_NEIGHBORS variable in the Ghost class.\n");
				break;
			}

			// Construct a list of nodes that could be moved to (Ghosts cannot move to the previous node)
			if ((*iter) != previousNode)
			{
				nodeIdArray[index] = (*iter)->GetNodeId();
				index++;
			}
		}

		// index is now the number of available directions
		int randNodeIndex = std::rand() % index;
		int targetNodeId = nodeIdArray[randNodeIndex];
		int currentNodeId = currentNode->GetNodeId();

		// Select a direction to move in based on the node ID
		// the nodes are generated in order from left to right & top to bottom
		if (targetNodeId == (currentNodeId + 1))
		{
			// Move right
			newDirection = DirectionEnum::Right;
		}
		else if (targetNodeId == (currentNodeId - 1))
		{
			// Move left
			newDirection = DirectionEnum::Left;
		}
		else if (targetNodeId < (currentNodeId - 1))
		{
			// Move up
			newDirection = DirectionEnum::Up;
		}
		else if (targetNodeId > (currentNodeId + 1))
		{
			// Move down
			newDirection = DirectionEnum::Down;
		}
	}

	// Update the AI direction when we are centered in a tile
	if ((isCenteredOnTile) && (newDirection != DirectionEnum::None))
	{
		position.x = (currentNode->GetLocation().x * G_SIZE);
		position.y = (currentNode->GetLocation().y * G_SIZE);
		direction = newDirection;
		newDirection = DirectionEnum::None;
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

	boundingRect.x = position.x;
	boundingRect.y = position.y;

	previousFrameNode = currentNode;
}

void Ghost::UpdateNodes(Node* newNode)
{
	previousNode = currentNode;
	currentNode = newNode;
}

void Ghost::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);

	// draw the bounding rectangle for degubbing
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
	//SDL_RenderDrawRect(renderer, &boundingRect);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
}

void Ghost::SetDirection(DirectionEnum dirEnum)
{
	previousDirection = direction;
	direction = dirEnum;
}

Node* Ghost::GetCurrentNode()
{
	return currentNode;
}

Node* Ghost::GetPreviousNode()
{
	return previousNode;
}

DirectionEnum Ghost::GetDirection()
{
	return direction;
}

void Ghost::SetPosition(Node* node)
{
	position.x = (G_SIZE * node->GetLocation().x) + (G_SIZE / 2);
	position.y = (G_SIZE * node->GetLocation().y) + (G_SIZE / 2);
	currentNode = node;
}
SDL_Rect* Ghost::GetBoundingRect()
{
	return &boundingRect;
}

void Ghost::SetPreviousDirection(DirectionEnum dir)
{
	previousDirection = dir;
}