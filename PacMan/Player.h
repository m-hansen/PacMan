#pragma once
#include <SDL.h>
#include "TextureManager.h"
#include "MovingEntity.h"
#include "DirectionEnum.h"
#include "Node.h"

class Player : public MovingEntity
{
public:
	Player();
	~Player();
	void Initialize();
	void Update(Uint32 deltaT);
	void UpdateNodes(Node* newNode);
	void Render(SDL_Renderer* renderer);
	void SetDirection(DirectionEnum dirEnum);
	bool Kill();
	SDL_Rect* GetBoundingRect();
	SDL_Rect* GetSpriteRect();
	Node* GetCurrentNode();
	Node* GetPreviousNode();
	void SetPosition(Node* node);
	DirectionEnum GetDirection();
	DirectionEnum GetPreviousDirection();
	void SetPreviousDirection(DirectionEnum dir);
	bool isCenteredOnTile;
private:
	bool isAlive;
	int livesLeft;
	SDL_Rect spriteRect;
	SDL_Rect livesLeftRect;
	SDL_Rect boundingRect;
	DirectionEnum direction;
	DirectionEnum previousDirection;
	Node* currentNode;
	Node* previousNode;
	SDL_Texture* texture;
};