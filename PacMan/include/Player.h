#pragma once
#include "SDL.h"
#include "TextureManager.h"
#include "Renderable.h"
#include "DirectionEnum.h"
#include "Node.h"

class Player : public Renderable
{
public:
	Player();
	~Player();
	void Initialize();
	void Update(Uint32 deltaT);
	void UpdateNodes(Node* newNode);
	void Render(SDL_Renderer* renderer);
	void SetDirection(DirectionEnum dirEnum);
	void QueueDirection(DirectionEnum dirEnum);
	bool Kill();
	SDL_Rect* GetBoundingRect();
	SDL_Rect* GetSpriteRect();
	Node* GetCurrentNode();
	Node* GetPreviousNode();
	void SetPosition(Node* node);
	DirectionEnum GetDirection();
	DirectionEnum GetPreviousDirection();
	void SetPreviousDirection(DirectionEnum dir);
	bool isAlignedWithTile;
private:
	void ConsumeQueuedMovement();
	bool isAlive;
	bool isVisible;
	int livesLeft;
	Vector2f position;
	float speed;
	SDL_Rect spriteRect;
	SDL_Rect livesLeftRect;
	SDL_Rect boundingRect;
	DirectionEnum direction;
	DirectionEnum previousDirection;
	DirectionEnum newDirection;
	Node* currentNode;
	Node* previousNode;
	SDL_Texture* texture;
	SDL_Texture* livesTexture;
	bool isMoving;
	bool isCenteredOnTile;
};