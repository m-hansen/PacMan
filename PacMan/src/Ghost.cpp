#include "Ghost.h"

GhostStateEnum Ghost::state;
GhostStateEnum Ghost::previousState;
GameTimer Ghost::stateTimer;

Ghost::Ghost(std::string textureName, float spawnX, float spawnY, DirectionEnum dir)
{
	fprintf(stdout, "Creating a new ghost\n");

	texture = TextureManager::GetTexture(textureName.c_str());
	frightenedTexture = TextureManager::GetTexture("frightened");

	// Set the location and size
	spawnPoint = { spawnX * GRID_SIZE, spawnY * GRID_SIZE};
	position = spawnPoint;
	boundingRect.w = GRID_SIZE;
	boundingRect.h = GRID_SIZE;
	boundingRect.x = position.x;
	boundingRect.y = position.y;

	defaultSpeed = 0.045f * (GRID_SIZE / 8);
	speed = defaultSpeed;

	state = GhostStateEnum::Scatter;
	previousState = state;
	isFrightened = false;

	// Set direction variables
	defaultDirection = dir;
	direction = defaultDirection;
	previousDirection = direction;
	queuedDirection = DirectionEnum::None;

	currentNode = NULL;
	previousNode = NULL;
	previousFrameNode = NULL;

	stateTimer.Start();
}

Ghost::~Ghost()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;
}

void Ghost::Respawn()
{
	// Reset the position
	position = spawnPoint;
	boundingRect.x = position.x;
	boundingRect.y = position.y;

	// Reset direction variables
	defaultDirection = defaultDirection;
	direction = defaultDirection;
	previousDirection = direction;
	queuedDirection = DirectionEnum::None;

	// Initialize the nodes
	currentNode = NULL;
	previousNode = NULL;
	previousFrameNode = NULL;

	// The ghost should never be frightened after a respawn
	// even if the overall state is frightened
	LeaveFrightenedState();
}

void Ghost::Update(Uint32 deltaT)
{
	CheckForStateChange();

	// Check if the AI is centered
	if (currentNode != NULL)
	{
		// Account for rounding errors
		float posX = (position.x - (currentNode->GetPosition().x));
		float posY = (position.y - (currentNode->GetPosition().y));
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
		// Set the previous node to the node we were in last frame
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
			queuedDirection = DirectionEnum::Right;
		}
		else if (targetNodeId == (currentNodeId - 1))
		{
			// Move left
			queuedDirection = DirectionEnum::Left;
		}
		else if (targetNodeId < (currentNodeId - 1))
		{
			// Move up
			queuedDirection = DirectionEnum::Up;
		}
		else if (targetNodeId > (currentNodeId + 1))
		{
			// Move down
			queuedDirection = DirectionEnum::Down;
		}
	}

	// Update the AI direction when we are centered in a tile
	if ((isCenteredOnTile) && (queuedDirection != DirectionEnum::None))
	{
		position.x = (currentNode->GetPosition().x);
		position.y = (currentNode->GetPosition().y);
		direction = queuedDirection;
		queuedDirection = DirectionEnum::None;
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

void Ghost::CheckForStateChange()
{
	Uint32 stateTimeLength = 0;

	// Set the time for each state
	if (state == Scatter)
		stateTimeLength = 7000;
	else if (state == Chase)
		stateTimeLength = 20000;
	else
		stateTimeLength = 5000;

	// Check if we have exceeded the time for the current state
	if (stateTimer.GetTicks() > stateTimeLength)
		NextState();

	if (state != Frightened)
		LeaveFrightenedState();
}

void Ghost::ReverseDirection()
{
	switch (direction)
	{
		case DirectionEnum::Up:
			queuedDirection = DirectionEnum::Down;
			break;
		case DirectionEnum::Down:
			queuedDirection = DirectionEnum::Up;
			break;
		case DirectionEnum::Left:
			queuedDirection = DirectionEnum::Right;
			break;
		case DirectionEnum::Right:
			queuedDirection = DirectionEnum::Left;
			break;
		default:
			queuedDirection = DirectionEnum::None;
	}
}

void Ghost::EnterFrightenedState(float percentSpeed)
{
	isFrightened = true;
	speed = defaultSpeed * percentSpeed;
}

void Ghost::LeaveFrightenedState()
{
	isFrightened = false;
	speed = defaultSpeed;
}

void Ghost::UpdateNodes(Node* newNode)
{
	previousNode = currentNode;
	currentNode = newNode;
}

void Ghost::Render(SDL_Renderer* renderer)
{
	if (isFrightened)
		SDL_RenderCopy(renderer, frightenedTexture, NULL, &boundingRect);
	else
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

/*void Ghost::SetPosition(Node* node)
{
	position.x = (node->GetPosition().x) + (GRID_SIZE / 2);
	position.y = (node->GetPosition().y) + (GRID_SIZE / 2);
	boundingRect.x = position.x;
	boundingRect.y = position.y;
	currentNode = node;
}*/

SDL_Rect* Ghost::GetBoundingRect()
{
	return &boundingRect;
}

void Ghost::SetPreviousDirection(DirectionEnum dir)
{
	previousDirection = dir;
}

bool Ghost::ChangeState(GhostStateEnum newState)
{
	// Don't update the state if we are already in the requested state
	if (newState == state)
		return false;

	fprintf(stdout, "AI changing states\n");

	stateTimer.Start();

	// Update the previous and current state
	previousState = state;
	state = newState;

	return true;
}

void Ghost::NextState()
{
	if (state == Scatter)
		ChangeState(Chase);
	else if (state == Chase)
		ChangeState(Scatter);
	else // frightened
		ChangeState(previousState);
}

char* Ghost::CurrentStateName() 
{ 
	switch (state)
	{
		case Scatter: return "Scatter";
		case Chase: return "Chase";
		case Frightened: return "Frightened";
	}
}
