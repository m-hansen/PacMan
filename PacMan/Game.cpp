#include "Game.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;

SDL_Rect pacmanSprite[4];

Game::Game()
{
	window = NULL;
	screenSurface = NULL;
	isRunning = true;
	isDebugging = false;
}

Game::~Game()
{
	delete (player);
	player = NULL;

	delete (tileMap);
	tileMap = NULL;
}

bool Game::Initialize()
{
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
	TextureManager::LoadTexture(renderer, "tile", "tile.png");
	TextureManager::LoadTexture(renderer, "pacman", "pac-man.png");

	//if (textureManager.LoadTexture(renderer, "sprite sheet", "pac-man-sprite-sheet.png"))
	//{
	//	// Split apart the sprite sheet
	//	int size = 15;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		pacmanSprite[i].x = size * i;
	//		pacmanSprite[i].y = 0;
	//		pacmanSprite[i].w = size;
	//		pacmanSprite[i].h = size;
	//	}

	//}
}

void Game::Run()
{
	// Allocate the tile map on the heap
	tileMap = new TileMap();
	tileMap->GenerateMap();

	player = new Player();
	player->Initialize();

	while (isRunning)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}

		Update();

		Render();
	}
}

void Game::Update()
{
	// Update the player
	player->Update();
}

void Game::Render()
{
	// Clear color
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
	SDL_RenderClear(renderer);

	//if (isDebugging) 
	//{
	//	int tileSize = 16;
	//	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	//	for (int i = 0; i < SCREEN_HEIGHT; i += tileSize)
	//	{
	//		for (int j = 0; j < SCREEN_WIDTH; j += tileSize)
	//		{
	//			// Draw vertical lines
	//			SDL_RenderDrawLine(renderer, j, i, j, SCREEN_HEIGHT);

	//			// Draw horizontal lines
	//			SDL_RenderDrawLine(renderer, j, i, SCREEN_WIDTH, i);
	//		}
	//	}
	//	
	//}

	// Render the tile map
	tileMap->Render(renderer);

	player->Render(renderer);

	//Set rendering space and render to screen
    /*SDL_Rect renderQuad = { x, y, mWidth, mHeight };*/

    //Set clip rendering dimensions
    /*if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }*/

	/*SDL_Texture* pacmanTexture = textureManager.GetTexture("sprite sheet");
	for (int i = 0; i < 4; i++)
	{
		pacmanTexture.render(0, 0, &pacmanSprite[i]);
	}*/

    //Render to screen
	//SDL_RenderCopy(renderer, TextureManager::GetTexture("pacman"), NULL, NULL);

	//SDL_RenderCopy(renderer, textureManager.GetTexture("sprite sheet"), NULL, NULL);

	// Update the screen
    SDL_RenderPresent(renderer);
}