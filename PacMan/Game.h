#pragma once
#include <stdio.h>
#include <SDL.h>
#include "TextureManager.h"
#include "TileMap.h"
#include "Player.h"
#include "DirectionEnum.h"

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
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;
	SDL_Event e;
	bool isRunning;
	bool isDebugging;
	TileMap* tileMap;
	Player* player;
};