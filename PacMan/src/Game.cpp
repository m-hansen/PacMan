#include "Game.h"
#include "IScreen.h"

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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
}

void Game::HandleEvents()
{
	screens.back()->HandleEvents(this);
}

void Game::Update()
{
	screens.back()->Update(this);
}

void Game::Render()
{
	screens.back()->Render(this);
}

void Game::ChangeScreen(IScreen* screen)
{
	// Cleanup the current screen
	if (!screens.empty())
	{
		screens.back()->Cleanup(this);
		screens.pop_back();
	}

	screens.push_back(screen);
	screens.back()->Initialize(this);
}

void Game::PushScreen(IScreen* screen)
{
	screens.push_back(screen);
	screens.back()->Initialize(this);
}

void Game::PopScreen()
{
	if (!screens.empty())
	{
		screens.back()->Cleanup(this);
		screens.pop_back();
	}
}

bool Game::Running()
{
	return isGameRunning;
}

void Game::Quit()
{
	isGameRunning = false;
}