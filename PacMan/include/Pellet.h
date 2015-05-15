#pragma once
#include "SDL.h"
#include "IRenderable.h"
#include "TextureManager.h"
#include "Node.h"
#include "GameTimer.h"

class Pellet : public IRenderable
{
public:
	Pellet(Node* node);
	~Pellet();
	void PlayBlinkAnimation(const int BLINK_RATE);
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect() { return &boundingRect; }
	NodeTypeEnum GetType() { return type; }
	const int GetValue() { return value; }
private:
	int value;
	void InitBoundingRect(int size);
	bool isVisible;
	Node* location;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
	NodeTypeEnum type;
	GameTimer blinkTimer;
};