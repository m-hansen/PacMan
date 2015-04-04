#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Config.h"
#include "TextureManager.h"
#include "Player.h"
#include "Enumerations/DirectionEnum.h"
#include "Wall.h"
#include "LevelManager.h"
#include "Graph.h"
#include "Ghost.h"
//#include "Screens/IScreen.h"
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
	Uint32 GetDeltaTime() { return deltaT; }
private:
	void CheckForJoysticks();
	Uint32 deltaT = 0;
	Uint32 previousTime = 0;
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Joystick* gameController;
	bool isGameRunning;
	std::vector<IScreen*> screens;
};