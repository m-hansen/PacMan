#pragma once
#include "SDL.h"
#include "MovingAgent.h"
#include "TextureManager.h"
#include "IRenderable.h"
#include "DirectionEnum.h"
#include "Node.h"

class Player : public IRenderable
{
public:
	Player();
	~Player();
	//void Initialize();
	void ResetPosition();
	void Update(Uint32 deltaT);
	void UpdateNodes(Node* newNode);
	void Render(SDL_Renderer* renderer);
	void SetDirection(DirectionEnum dirEnum);
	void QueueDirection(DirectionEnum dirEnum);
	void Kill();
	SDL_Rect* GetBoundingRect();
	SDL_Rect* GetSpriteRect();
	Node* GetCurrentNode();
	Node* GetPreviousNode();
	void SetPosition(Node* node);
	DirectionEnum GetDirection();
	DirectionEnum GetPreviousDirection();
	void SetPreviousDirection(DirectionEnum dir);
	bool isAlignedWithTile;
	bool IsAlive() { return isAlive; }
private:
	void ConsumeQueuedMovement();
	bool isAlive;
	bool isVisible;
	Vector2f position;
	float speed;
	SDL_Rect spriteRect;
	SDL_Rect boundingRect;
	DirectionEnum direction;
	DirectionEnum previousDirection;
	DirectionEnum queuedDirection;
	Node* currentNode;
	Node* previousNode;
	SDL_Texture* texture;
	bool isMoving;
	bool isCenteredOnTile;
};