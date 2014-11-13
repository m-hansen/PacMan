#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <SDL.h>
#include "TextureManager.h"
#include "TileMap.h"
#include "Player.h"
#include "DirectionEnum.h"
#include "Wall.h"
#include "Graph.h"

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
	void Render();
	void Run();
	bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;
	SDL_Event previousEvent;
	SDL_Event currentEvent;
	bool isRunning;
	bool isDebugging;
	TileMap* tileMap;
	Player* player;
	Graph* level;
	Uint32 deltaT = 0;
	Uint32 previousTime = 0;
};