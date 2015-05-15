#include "Game.h"
#include "Screens/IScreen.h"

Game::Game()
{
	window = NULL;
	screenSurface = NULL;
	gameController = NULL;
	isGameRunning = true;
}

Game::~Game()
{
	// Close game controller
	SDL_JoystickClose(gameController);
	gameController = NULL;

	// Destroy the renderer and window    
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL and its subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::Initialize()
{
	// Load the configuration file
	std::string configFile = "Resources/config.xml";
	Config::LoadConfig(configFile);

	// Initialize fonts
	if (TTF_Init() != 0)
	{
		printf("Failed to initialize SDL_ttf\n");
		return false;
	}
	
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initailize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	
	// Create a window
	window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Config::screenWidth, Config::screenHeight, SDL_WINDOW_SHOWN);
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

	// Check for joysticks and gamepads
	CheckForJoysticks();

	return true;
}

void Game::CheckForJoysticks()
{
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n");
	}
	else
	{
		// Load joysticks
		gameController = SDL_JoystickOpen(0);
		if (gameController == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}
}

void Game::LoadContent()
{
	// Create the font
	//sansFont = TTF_OpenFont("Fonts/ARIAL.TTF", 24);
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
	// Calculate delta time
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime > previousTime)
	{
		deltaT = currentTime - previousTime;
		previousTime = currentTime;
	}

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