#include "Game.h"

const int SCREEN_WIDTH = 28 * G_SIZE; // 224 width with 8x8 tiles
const int SCREEN_HEIGHT = 36 * G_SIZE; // 288 height with 8x8 tiles

Game::Game()
{
	window = NULL;
	screenSurface = NULL;
	level = NULL;
	sansFont = NULL;
	scoreTexture = NULL;
	isRunning = true;
	isDebugging = true;
	score = 0;
}

Game::~Game()
{
	// TODO free font texture
	//TTF_CloseFont(sansFont);
	sansFont = NULL;

	delete (player);
	player = NULL;

	delete (level);
	level = NULL;

	//delete (tileMap);
	//tileMap = NULL;

	// Destroy the renderer and window    
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
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
	// Create the font
	//sansFont = TTF_OpenFont("Fonts\\ARIAL.TTF", 24);
	/*SDL_Surface* scoreSurface = TTF_RenderText_Solid(sansFont, "Score: " + score, SDL_Color{255, 255, 255});
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);*/

	// Load textures
	TextureManager::LoadTexture(renderer, "tile", "tile.png");
	TextureManager::LoadTexture(renderer, "pacman", "pac-man.png");
	TextureManager::LoadTexture(renderer, "wall", "wall.png");
	TextureManager::LoadTexture(renderer, "pellet", "pellet.png");
}

void Game::Run()
{
	levelManager.LoadLevel("Level1.txt");
	// Allocate the tile map on the heap
	//tileMap = new TileMap();
	//tileMap->GenerateMap();

	// Load level test
	//tileMap->LoadLevel("pac-man-board.txt");
	level = new Graph();
	level->GenerateGraph(SCREEN_WIDTH / G_SIZE, SCREEN_HEIGHT / G_SIZE);
	score = 0;

	levelManager.FindEdges();

	player = new Player();
	player->Initialize();
	currentNode = NULL;
	previousNode = NULL;
	
	while (isRunning)
	{
		// Handle events on queue
		while (SDL_PollEvent(&currentEvent) != 0)
		{
			// User requests quit
			if (currentEvent.type == SDL_QUIT)
			{
				isRunning = false;
			}
			// User presses a key
			else if (currentEvent.type == SDL_KEYDOWN)
			{
				switch (currentEvent.key.keysym.sym)
				{
					case SDLK_UP:
						player->SetDirection(Up);
						break;
					case SDLK_DOWN:
						player->SetDirection(Down);
						break;
					case SDLK_LEFT:
						player->SetDirection(Left);
						break;
					case SDLK_RIGHT:
						player->SetDirection(Right);
						break;
					case SDLK_F3:
						// Toggle debugging information
						isDebugging = !isDebugging;
						break;
					case SDLK_ESCAPE:
						// User requests quit
						isRunning = false;
						break;
				}
			}
			// User clicks the mouse
			else if (currentEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (currentEvent.button.clicks)
				{
					case SDL_BUTTON_LEFT:
						printf("Left mouse button clicked at\tx=%d\ty=%d\n", currentEvent.button.x, currentEvent.button.y);
						break;
					case SDL_BUTTON_RIGHT:
						break;
					case SDL_BUTTON_MIDDLE:
						break;
				}
			}

			previousEvent = currentEvent;
		}

		Update();
		
		HandleCollisions();

		Render();
	}
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

	// Update the player
	player->Update(deltaT);
}

void Game::HandleCollisions()
{
	// Check which node the player occupies
	for (std::vector<Node*>::iterator iter = levelManager.legalPlayingNodes.begin();
		iter != levelManager.legalPlayingNodes.end(); ++iter)
	{
		// Check if player is insid a node
		if (CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
			player->UpdateNodes(*iter);
			if (player->GetCurrentNode() != player->GetPreviousNode())
			{
				// Print the node ID that the player enters
				printf("Player is in node ID : %d\n", (*iter)->GetNodeId());
			}

		}
	}

	// Check for collisions between the player and the wall
	for (std::vector<Wall*>::iterator iter = levelManager.wallList.begin(); iter != levelManager.wallList.end(); ++iter)
	{
		// Check if the player collides with a wall
		if (CollisionChecker(player->GetSpriteRect(), (*iter)->GetBoundingRect()))
		{
			printf("A collision has occured between the player and a wall!\n");
		}
	}

	// Check for collisions between player and consumable
	// NOTE: we must use iter++ instead of ++iter
	for (std::vector<Pellet*>::iterator iter = levelManager.pelletList.begin(); iter != levelManager.pelletList.end(); iter++)
	{
		if (CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
			// Free memory, clear list, and increment score
			delete (*iter);
			levelManager.pelletList.erase(iter);
			score += 100;
			printf("Score: %d\n", score);
			// The break is necessary for two reasons
			// 1) we stop checking for nodes once we found we collided with one
			// 2) if we allow iter to increment we will crash because we are erasing elements
			break;
		}
	}

}

void Game::Render()
{
	// Clear color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(renderer);

	// Render the tile map
	//tileMap->Render(renderer);

	// Render each node in the level
	if (isDebugging)
	{
		// Iterate over each node in the level graph
		//for (std::vector<Node*>::iterator iter = level->GetAllNodes()->begin(); iter != level->GetAllNodes()->end(); ++iter)
		for (std::vector<Node*>::iterator iter = levelManager.legalPlayingNodes.begin(); 
			iter != levelManager.legalPlayingNodes.end(); ++iter)
		{
			Node* node = (*iter);
			Vector2f loc = node->GetLocation();

			// Temporarily set the rendering color to white for the nodes
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
			switch (node->GetType())
			{
			//case NodeType::Empty:
				//SDL_RenderCopy(renderer, TextureManager::GetTexture(, NULL, &boundingRect);
			}
			SDL_RenderDrawPoint(renderer, loc.x * G_SIZE, loc.y * G_SIZE);

			// Draw each node's bounding rectangle
			(*iter)->Render(renderer);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
		}
	}

	// Render the walls
	for (std::vector<Wall*>::iterator iter = levelManager.wallList.begin();
		iter != levelManager.wallList.end(); ++iter)
	{
		(*iter)->Render(renderer);
	}

	// Render each remaining pellet on the board
	for (std::vector<Pellet*>::iterator iter = levelManager.pelletList.begin();
		iter != levelManager.pelletList.end(); ++iter)
	{
		(*iter)->Render(renderer);
	}

	// Render the player
	player->Render(renderer);

	// Display the score
	//SDL_Draw

	// Update the screen
    SDL_RenderPresent(renderer);
}

bool Game::CollisionChecker(SDL_Rect* a, SDL_Rect* b)
{
	// Set the bounds for collider A
	int leftA = a->x;
	int rightA = a->x + a->w;
	int topA = a->y;
	int bottomA = a->y + a->h;

	// Set the bounds for collider B
	int leftB = b->x;
	int rightB = b->x + b->w;
	int topB = b->y;
	int bottomB = b->y + b->h;

	// Check for collision
	// if any of these statements are true we know that there is a gap between 
	// the two colliders on the horizontal or vertical axis
	if ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB))
	{
		return false;
	}
	
	// Both the horizontal and vertical coordinates have an overlap
	return true;
}