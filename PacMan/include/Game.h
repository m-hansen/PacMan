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
//#include "IScreen.h"
class IScreen;

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void LoadContent();
	void HandleEvents();
	void Update();
	void Render();
	void ChangeScreen(IScreen* screen);
	void PushScreen(IScreen* screen);
	void PopScreen();
	bool Running();
	void Quit();
	SDL_Renderer* renderer;
private:
	SDL_Window* window;
	SDL_Surface* screenSurface;
	bool isGameRunning;
	std::vector<IScreen*> screens;
};