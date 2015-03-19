#pragma once
#include "SDL.h"
#include "MovingAgent.h"
#include "Global.h"
#include "Utils.h"
#include "TextureManager.h"
#include "IRenderable.h"
#include "Node.h"
#include "DirectionEnum.h"
#include "GhostStateEnum.h"
#include "GameTimer.h"
#include "Sprite.h"
#include "Pathfinder.h"

class Ghost : public IRenderable
{
public:
	Ghost(std::string textureName, float spawnX, float spawnY, 
		Pathfinder* pf, Node* scatterNode, DirectionEnum dir);
	~Ghost();
	void Respawn();
	void FollowPath();
	void Update(Uint32 deltaT);
	void ScatterMovement();
	void ChaseMovement();
	void FrightenedMovement();
	void CheckForStateChange();
	void EnterFrightenedState(float percentSpeed);
	void LeaveFrightenedState();
	void ReverseDirection();
	void UpdateNodes(Node* newNode);
	void Render(SDL_Renderer* renderer);
	void MoveForward();
	SDL_Rect* GetBoundingRect();
	DirectionEnum GetDirection();
	void SetDirection(DirectionEnum dirEnum);
	Node* GetCurrentNode();
	Node* GetPreviousNode();
	//void SetPosition(Node* node);
	bool IsFrightened() { return isFrightened; }
	void SetPreviousDirection(DirectionEnum dir);
	static GhostStateEnum CurrentState() { return state; }
	static char* CurrentStateName();
	static bool ChangeState(GhostStateEnum newState);
	static void NextState();
	static GameTimer& GetStateTimer() { return stateTimer; }
private:
	// Functions
	void QueueDirectionTowardNode(Node* target);
	void CenterOnCurrentNode();
	bool IsAtIntersection();

	// Variables
	static GhostStateEnum state;
	static GhostStateEnum previousState;
	static GameTimer stateTimer;
	const static int SCATTER_DURATION = 7000;
	const static int CHASE_DURATION = 20000;
	const static int FRIGHTENED_DURATION = 5000;
	SDL_Rect boundingRect;
	SDL_Texture* texture;
	SDL_Texture* frightenedTexture;
	Vector2f spawnPoint;
	Vector2f position;
	DirectionEnum direction;
	DirectionEnum previousDirection;
	DirectionEnum queuedDirection;
	DirectionEnum defaultDirection;
	float defaultSpeed;
	float speed;
	Node* scatterNode;
	Node* currentNode;
	Node* previousNode;
	Node* previousFrameNode;
	bool isCenteredOnTile;
	bool isFrightened;
	std::vector<Node*> pathStack;
	Pathfinder* pf;
};