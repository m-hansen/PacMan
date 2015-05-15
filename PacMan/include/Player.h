#pragma once
#include "SDL.h"
#include "MovingAgent.h"
#include "TextureManager.h"
#include "IRenderable.h"
#include "Enumerations/DirectionEnum.h"
#include "Node.h"
#include "Sprite.h"
#include "GameTimer.h"

const int MOVEMENT_FRAMES = 8;
const int DEATH_FRAMES = 12;

class Player
{
public:
	Player();
	~Player();
	//void Initialize();
	void ClipSpriteSheets();
	void ResetPosition();
	void Update(Uint32 deltaT);
	void AnimateMovement();
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
	bool HasDeathAnimationFinished() { return hasDeathAnimationFinished; }
	bool IsAlive() { return isAlive; }
private:
	// Used to hold the starting index for the player's
	// movement animations
	enum MovementFrameDirectionIndex {
		RightFrameIndex = 0, LeftFrameIndex = 2,
		UpFrameIndex = 4, DownFrameIndex = 6
	};
	void ConsumeQueuedMovement();
	bool isAlive;						// true if the player is still alive
	bool isVisible;						// true if the player is actively being rendered
	bool hasDeathAnimationFinished;
	Sprite* sprite;
	Vector2f position;					// location of the player
	Vector2f spawnPoint;				// location that the player should spawn at when created or reset
	float speed;						// rate the player moves at
	SDL_Rect spriteRect;				// the rectangle that the texture is rendered to
	SDL_Rect boundingRect;				// the rectangle used for collisions
	DirectionEnum direction;			// the current direction
	DirectionEnum previousDirection;	// the last direction that is not the current direction
	DirectionEnum queuedDirection;		// the direction to take when the player arrives at an intersection
	Node* currentNode;					// the current node the player is in
	Node* previousNode;					// the last node the player was is that is not the current node
	SDL_Texture* texture;				// the texture used for rendering the player
	bool isMoving;						// true if the player is moving in any direction
	bool isCenteredOnTile;				// true if the player is centered in the middle of a tile
	// Animations
	SDL_Rect movementAnimation[MOVEMENT_FRAMES]; // container for all movement related clips
	SDL_Rect deathAnimation[DEATH_FRAMES]; // container for all death related clips
	SDL_Rect* currentClip;				// the current clip in the animation
	int animationFrame;					// the current frame of the animation that is playing
	int directionFrameIndex;			// the starting index based on the direction the player is facing
	GameTimer animationTimer;
};