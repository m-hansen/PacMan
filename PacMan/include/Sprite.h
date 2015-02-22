#pragma once
#include "SDL.h"
#include "Utils.h"
#include "Global.h"

class Sprite
{
public:
	Sprite(SDL_Texture* t, float x, float y, float w, float h);
	~Sprite();
	void Render(SDL_Renderer* renderer);
	SDL_Rect* GetBoundingRect() { return &boundingRect; }
	void SetBounds(float w, float h);
	void SetPosition(float x, float y);
	void SetTexture(SDL_Texture* t) { texture = t; }
	void Show() { isVisible = true; }
	void Hide() { isVisible = false; }
	bool Visible() { return isVisible; }
private:
	Vector2f origin;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
	bool isVisible;
};