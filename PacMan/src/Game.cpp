#include "Game.h"

const int SCREEN_WIDTH = 28 * G_SIZE; // 224 width with 8x8 tiles
const int SCREEN_HEIGHT = 36 * G_SIZE; // 288 height with 8x8 tiles

Game::Game()
{
	window = NULL;
	screenSurface = NULL;
	isGameRunning = true;
}

Game::~Game()
{
	// Destroy the renderer and window    
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
}

bool Game::Initialize()
{
	// Initialize fonts
	if (TTF_Init() != 0)
	{
		printf("Failed to initialize SDL_ttf\n");
		return false;
	}

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initailize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	// Create a window
	window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Get the window surface
	screenSurface = SDL_GetWindowSurface(window);

	// Create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

	return true;
}

void Game::LoadContent()
{
	// Create the font
	//sansFont = TTF_OpenFont("Fonts\\ARIAL.TTF", 24);
	/*SDL_Surface* scoreSurface = TTF_RenderText_Solid(sansFont, "Score: " + score, SDL_Color{255, 255, 255});
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);*/

	// Load textures
	TextureManager::LoadTexture(renderer, "tile", "Resources\\tile.png");
	TextureManager::LoadTexture(renderer, "pacmanRight", "Resources\\pac-man_R.png");
	TextureManager::LoadTexture(renderer, "pacmanLeft", "Resources\\pac-man_L.png");
	TextureManager::LoadTexture(renderer, "pacmanUp", "Resources\\pac-man_U.png");
	TextureManager::LoadTexture(renderer, "pacmanDown", "Resources\\pac-man_D.png");
	TextureManager::LoadTexture(renderer, "pacmanClosed", "Resources\\pac-man_closed.png");
	TextureManager::LoadTexture(renderer, "wall", "Resources\\wall.png");
	TextureManager::LoadTexture(renderer, "pellet", "Resources\\pellet.png");
	TextureManager::LoadTexture(renderer, "blinky", "Resources\\blinky.png");
	TextureManager::LoadTexture(renderer, "pinky", "Resources\\pinky.png");
	TextureManager::LoadTexture(renderer, "inky", "Resources\\inky.png");
	TextureManager::LoadTexture(renderer, "clyde", "Resources\\clyde.png");
}

void Game::Run()
{
	IScreen* currentScreen = new GameplayScreen(renderer);

	while (isGameRunning)
	{
		currentScreen->HandleEvents();
		currentScreen->Update();
		currentScreen->Render();
	}
}