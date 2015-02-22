#pragma once
#include "SDL.h"
#include "IRenderable.h"
#include "TextureManager.h"
#include "Node.h"

class Pellet : public IRenderable
{
public:
	Pellet(Node* node);
	~Pellet();
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect() { return &boundingRect; }
	NodeTypeEnum GetType() { return type; }
	const int GetValue() { return value; }
private:
	int value;
	void InitBoundingRect(int size);
	Node* location;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
	NodeTypeEnum type;
};