#pragma once
#include "SDL.h"
#include "Node.h"
#include "Utils.h"
#include "Enumerations/DirectionEnum.h"

class MovingAgent
{
public:
	void Respawn();
	Node* GetCurrentNode() { return currentNode; }
	Node* GetPreviousNode() { return previousNode; }
protected:
	SDL_Rect boundingRect;
	SDL_Texture* texture;
	Vector2f spawnPoint;
	Vector2f position;
	DirectionEnum direction;
	DirectionEnum previousDirection;
	DirectionEnum queuedDirection;
	DirectionEnum defaultDirection;
	float speed;
	Node* currentNode;
	Node* previousNode;
	Node* previousFrameNode;
};