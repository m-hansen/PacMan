#pragma once
#include "SDL_ttf.h"
#include "Player.h"
#include "LevelManager.h"
#include "Ghost.h"
#include "Screens/IScreen.h"
#include "Screens/AttractScreen.h"
#include "Pathfinder.h"
#include "HighScoreTable.h"

enum NodeFlags{ID, G, H, F};

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
	void DrawDebug(SDL_Renderer* renderer);
	void HandleCollisions();
	bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
	static GameplayScreen* Instance() { return &gameplayScreen; }
private:
	void LoadContent(SDL_Renderer* renderer);
	void PlayerDeath();
	void GameEnd(int condition);
	static GameplayScreen gameplayScreen;
	//Player* player;
	LevelManager* levelManager;
	Pathfinder* pathfinder;
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
	bool isGameOver;
	TTF_Font* arialFont;
	SDL_Texture* scoreTexture;
	SDL_Rect scoreTextRect;
	SDL_Surface* scoreFontSurface;
	int score;
	int nodeDisplayFlags[4];

	// GUI variables
	SDL_Texture* pauseTexture;
	SDL_Texture* livesTexture;
	SDL_Rect livesLeftRect;
};