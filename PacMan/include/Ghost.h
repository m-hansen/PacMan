#pragma once
#include <SDL.h>
#include "Global.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Node.h"
#include "DirectionEnum.h"

class Ghost
{
public:
	Ghost(std::string textureName, float spawnX, float spawnY, DirectionEnum dir);
	~Ghost();
	void Update(Uint32 deltaT);
	void UpdateNodes(Node* newNode);
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect();
	DirectionEnum GetDirection();
	void SetDirection(DirectionEnum dirEnum);
	Node* GetCurrentNode();
	Node* GetPreviousNode();
	void SetPosition(Node* node);
	void SetPreviousDirection(DirectionEnum dir);
private:
	SDL_Rect boundingRect;
	SDL_Texture* texture;
	Vector2f position;
	Node* location;
	DirectionEnum direction;
	DirectionEnum previousDirection;
	DirectionEnum newDirection;
	float speed;
	Node* currentNode;
	Node* previousNode;
	Node* previousFrameNode;
	bool isCenteredOnTile;
};