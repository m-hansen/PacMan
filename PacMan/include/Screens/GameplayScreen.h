#pragma once
#include "SDL_ttf.h"
#include "Player.h"
#include "LevelManager.h"
#include "Ghost.h"
#include "IScreen.h"
#include "AttractScreen.h"

class GameplayScreen : public IScreen
{
public:
	~GameplayScreen();
	void Pause();
	void Resume();
	void Initialize(Game* game);
	void Cleanup(Game* game);
	//void InitializeLevel(std::string lvlName);
	void HandleEvents(Game* game);
	void Update(Game* game);
	void Render(Game* game);
	void RenderGUI(SDL_Renderer* renderer);
	void HandleCollisions();
	bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
	static GameplayScreen* Instance() { return &gameplayScreen; }
private:
	void LoadContent(SDL_Renderer* renderer);
	static GameplayScreen gameplayScreen;
	//Player* player;
	LevelManager* levelManager;
	int livesRemaining;
	//Ghost* blinky;
	//Ghost* pinky;
	//Ghost* inky;
	//Ghost* clyde;
	//std::vector<Ghost*> ghostList;
	std::string endGameMessage;
	bool isLevelOver;
	SDL_Event previousEvent;
	SDL_Event currentEvent;
	bool isRunning;
	bool isDebugging;
	bool isPaused;
	TTF_Font* arialFont;
	SDL_Texture* scoreTexture;
	SDL_Rect scoreTextRect;
	SDL_Surface* scoreFontSurface;
	int score;

	// GUI variables
	SDL_Texture* livesTexture;
	SDL_Rect livesLeftRect;
};