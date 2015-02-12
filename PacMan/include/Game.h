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
#include "IScreen.h"
#include "AttractScreen.h"
#include "GameplayScreen.h"

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
	void Run();
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;
	bool isGameRunning;
};