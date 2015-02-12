#pragma once
#include "SDL_ttf.h"
#include "Player.h"
#include "LevelManager.h"
#include "Ghost.h"
#include "IScreen.h"

class GameplayScreen : public IScreen
{
public:
	GameplayScreen(SDL_Renderer* rend);
	~GameplayScreen();
	void InitializeLevel(std::string lvlName);
	void HandleEvents();
	void Update();
	void HandleCollisions();
	void Render();
	bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
private:
	Graph* level;
	Player* player;
	Uint32 deltaT = 0;
	Uint32 previousTime = 0;
	LevelManager levelManager;
	Ghost* blinky;
	Ghost* pinky;
	Ghost* inky;
	Ghost* clyde;
	std::vector<Ghost*> ghostList;
	std::string endGameMessage;
	bool isLevelOver;
	SDL_Event previousEvent;
	SDL_Event currentEvent;
	bool isRunning;
	bool isDebugging;
	//SDL_Renderer* renderer;
	TTF_Font* arialFont;
	SDL_Texture* scoreTexture;
	SDL_Rect scoreTextRect;
	SDL_Surface* scoreFontSurface;
	int score;
};