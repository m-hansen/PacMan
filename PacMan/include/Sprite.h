#pragma once
#include "SDL.h"
#include "Utils.h"
#include "Global.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	void Render(SDL_Renderer* renderer);
	void SetBounds(float w, float h);
	void SetPosition(float x, float y);
	void SetTexture(SDL_Texture* t) { texture = t; }
	void Show() { isVisible = true; }
	void Hide() { isVisible = false; }
	bool Visible() { return isVisible; }
private:
	Vector2f localOrigin;
	SDL_Texture* texture;
	SDL_Rect boundingRect;
	bool isVisible;
};