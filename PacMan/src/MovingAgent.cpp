#include "MovingAgent.h"

void MovingAgent::Respawn()
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
}