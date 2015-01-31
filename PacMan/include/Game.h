#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Global.h"
#include "TextureManager.h"
#include "Player.h"
#include "DirectionEnum.h"
#include "Wall.h"
#include "LevelManager.h"
#include "Graph.h"
#include "Ghost.h"

enum GameStateEnum
{
	Attract,
	Options,
	Gameplay,
	Gameover,
	Pause
};

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void LoadContent();
	void Update();
	void HandleCollisions();
	void Render();
	void Run();
	void InitializeLevel(std::string lvlName);
	bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;
	SDL_Event previousEvent;
	SDL_Event currentEvent;
	bool isRunning;
	bool isDebugging;
	Player* player;
	Ghost* blinky;
	Ghost* pinky;
	Ghost* inky;
	Ghost* clyde;
	std::vector<Ghost*> ghostList;
	Graph* level;
	Uint32 deltaT = 0;
	Uint32 previousTime = 0;
	int score;
	LevelManager levelManager;
	std::vector<Consumable> consumableList;
	TTF_Font* arialFont;
	SDL_Texture* scoreTexture;
	SDL_Rect scoreTextRect;
	SDL_Surface* scoreFontSurface;
	std::string endGameMessage;
	bool isLevelOver;
};