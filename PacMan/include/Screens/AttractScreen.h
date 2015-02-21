#pragma once
#include <string>
#include "SDL_ttf.h"
#include "IScreen.h"
#include "Sprite.h"

class AttractScreen : public IScreen
{
public:
	~AttractScreen();
	void Initialize(Game* game);
	void Cleanup(Game* game);
	void HandleEvents(Game* game);
	void Update(Game* game);
	void Render(Game* game);
	static AttractScreen* Instance() { return &attractScreen; }
private:
	static AttractScreen attractScreen;
	SDL_Event previousEvent;
	SDL_Event currentEvent;
	bool isRunning;
	TTF_Font* titleFont;
	TTF_Font* instructionFont;
	Sprite title;
	Sprite instructions;
};