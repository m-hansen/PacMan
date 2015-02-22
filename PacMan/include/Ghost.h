#pragma once
#include "SDL.h"
#include "MovingAgent.h"
#include "Global.h"
#include "Utils.h"
#include "TextureManager.h"
#include "IRenderable.h"
#include "Node.h"
#include "DirectionEnum.h"

class Ghost : public MovingAgent, public IRenderable
{
public:
	Ghost(std::string textureName, float spawnX, float spawnY, DirectionEnum dir);
	~Ghost();
	//void Respawn();
	void Update(Uint32 deltaT);
	void ReverseDirection();
	void UpdateNodes(Node* newNode);
	void Render(SDL_Renderer* renderer);
	
	SDL_Rect* GetBoundingRect();
	DirectionEnum GetDirection();
	void SetDirection(DirectionEnum dirEnum);
	/*Node* GetCurrentNode();
	Node* GetPreviousNode();*/
	void SetPosition(Node* node);
	void SetPreviousDirection(DirectionEnum dir);
private:
	/*SDL_Rect boundingRect;
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
	Node* previousFrameNode;*/
	bool isCenteredOnTile;
};