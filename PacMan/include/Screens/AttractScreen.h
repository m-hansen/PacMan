#pragma once
#include <string>
#include "SDL_ttf.h"
#include "IScreen.h"

class AttractScreen : public IScreen
{
public:
	AttractScreen(SDL_Renderer* renderer);
	~AttractScreen();
	void HandleEvents();
	void Update();
	void Render();
private:
	SDL_Event previousEvent;
	SDL_Event currentEvent;
	bool isRunning;
	TTF_Font* titleFont;
	TTF_Font* instructionFont;
	SDL_Texture* titleTexture;
	SDL_Texture* instructionTexture;
	SDL_Rect titleTextRect;
	SDL_Rect instrTextRect;
};