#include "Game.h"

const int SCREEN_WIDTH = 28 * G_SIZE; // 224 width with 8x8 tiles
const int SCREEN_HEIGHT = 36 * G_SIZE; // 288 height with 8x8 tiles

Game::Game()
{
	window = NULL;
	screenSurface = NULL;
	level = NULL;
	arialFont = NULL;
	scoreTexture = NULL;
	isRunning = true;
	isDebugging = false;
	score = 0;
	scoreTextRect.w = G_SIZE * 6;
	scoreTextRect.h = G_SIZE * 1.5;
	scoreTextRect.x = 4 * G_SIZE;
	scoreTextRect.y = 32 * G_SIZE;
	endGameMessage = "Game Over!";
	isLevelOver = false;
}

Game::~Game()
{
	TTF_CloseFont(arialFont);
	arialFont = NULL;

	delete (player);
	player = NULL;

	delete (level);
	level = NULL;

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
	TextureManager::LoadTexture(renderer, "tile", "tile.png");
	TextureManager::LoadTexture(renderer, "pacmanRight", "pac-man_R.png");
	TextureManager::LoadTexture(renderer, "pacmanLeft", "pac-man_L.png");
	TextureManager::LoadTexture(renderer, "pacmanUp", "pac-man_U.png");
	TextureManager::LoadTexture(renderer, "pacmanDown", "pac-man_D.png");
	TextureManager::LoadTexture(renderer, "pacmanClosed", "pac-man_closed.png");
	TextureManager::LoadTexture(renderer, "wall", "wall.png");
	TextureManager::LoadTexture(renderer, "pellet", "pellet.png");
	TextureManager::LoadTexture(renderer, "blinky", "blinky.png");
	TextureManager::LoadTexture(renderer, "pinky", "pinky.png");
	TextureManager::LoadTexture(renderer, "inky", "inky.png");
	TextureManager::LoadTexture(renderer, "clyde", "clyde.png");
}

void Game::InitializeLevel(std::string lvlName)
{
	// Load the level, create the nodes, and determine the edges
	levelManager.LoadLevel(lvlName.c_str());
	level = new Graph();
	level->GenerateGraph(SCREEN_WIDTH / G_SIZE, SCREEN_HEIGHT / G_SIZE);
	levelManager.FindEdges();

	// Initialize the player
	player = new Player();
	player->Initialize();

	// Load the AI
	blinky = new Ghost("blinky");
	pinky = new Ghost("pinky");
	inky = new Ghost("inky");
	clyde = new Ghost("clyde");

	ghostList.push_back(blinky);
	ghostList.push_back(pinky);
	ghostList.push_back(inky);
	ghostList.push_back(clyde);

	score = 0;
}

void Game::Run()
{
	InitializeLevel("Level1.txt");

	// Load the default font
	arialFont = TTF_OpenFont("arial.ttf", G_SIZE);

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
						if (player->isCenteredOnTile && player->GetDirection() != DirectionEnum::Up)
						{
							player->SetPosition(player->GetCurrentNode());
							player->SetDirection(Up);
						}
						break;
					case SDLK_DOWN:
						if (player->isCenteredOnTile && player->GetDirection() != DirectionEnum::Down)
						{
							player->SetPosition(player->GetCurrentNode());
							player->SetDirection(Down);
						}
						break;
					case SDLK_LEFT:
						if (player->isCenteredOnTile && player->GetDirection() != DirectionEnum::Left)
						{
							player->SetPosition(player->GetCurrentNode());
							player->SetDirection(Left);
						}
						break;
					case SDLK_RIGHT:
						if (player->isCenteredOnTile && player->GetDirection() != DirectionEnum::Right)
						{
							player->SetPosition(player->GetCurrentNode());
							player->SetDirection(Right);
						}
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

	// Check for victory condition
	if (levelManager.pelletList.empty())
	{
		endGameMessage = "Congratulations!";
		isLevelOver = true;
		return;
	}

	// Update the player
	player->Update(deltaT);

	// Update the AI
	for (std::vector<Ghost*>::iterator iter = ghostList.begin(); iter != ghostList.end(); ++iter)
	{
		(*iter)->Update(deltaT);
	}
}

void Game::HandleCollisions()
{
	// Check which node the player/AI occupies
	for (std::vector<Node*>::iterator iter = levelManager.legalPlayingNodes.begin();
		iter != levelManager.legalPlayingNodes.end(); ++iter)
	{
		// Check if player is inside a node
		if (CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
			player->UpdateNodes(*iter);
			if (player->GetCurrentNode() != player->GetPreviousNode())
			{
				// Print the node ID that the player enters
				printf("Player is in node ID : %d\n", (*iter)->GetNodeId());
				player->SetPreviousDirection(player->GetDirection());
			}

		}

		// Iterate over each ghost
		for (std::vector<Ghost*>::iterator aiIter = ghostList.begin(); aiIter != ghostList.end(); ++aiIter)
		{
			// Check if a ghost is inside a node
			if (CollisionChecker((*aiIter)->GetBoundingRect(), (*iter)->GetBoundingRect()))
			{
				(*aiIter)->UpdateNodes(*iter);
				if ((*aiIter)->GetCurrentNode() != (*aiIter)->GetPreviousNode())
				{
					// Print the node ID that the player enters
					//printf("blinky is in node ID : %d\n", (*iter)->GetNodeId());
					(*aiIter)->SetPreviousDirection((*aiIter)->GetDirection());
				}
			}
		}
	}

	// Check for collisions between the player/AI and the wall
	for (std::vector<Wall*>::iterator iter = levelManager.wallList.begin(); iter != levelManager.wallList.end(); ++iter)
	{
		// Check if the player collides with a wall
		if (CollisionChecker(player->GetSpriteRect(), (*iter)->GetBoundingRect()))
		{
			if (((player->GetDirection() == DirectionEnum::Up) || (player->GetDirection() == DirectionEnum::Down))
				&& ((player->GetPreviousDirection() == DirectionEnum::Left) || (player->GetPreviousDirection() == DirectionEnum::Right)))
			{
				player->SetDirection(player->GetPreviousDirection());
			}
			else if (((player->GetDirection() == DirectionEnum::Left) || (player->GetDirection() == DirectionEnum::Right))
				&& ((player->GetPreviousDirection() == DirectionEnum::Up) || (player->GetPreviousDirection() == DirectionEnum::Down)))
			{
				player->SetDirection(player->GetPreviousDirection());
			}
			else
			{
				player->SetDirection(DirectionEnum::None);
				
			}
			printf("A collision has occured between the player and a wall!\n");
			//if (player->GetDirection() != player->GetPreviousDirection())
			{
				//player->SetDirection(player->GetPreviousDirection());
			}
			//else
			{
				//player->SetDirection(DirectionEnum::None);
			}
			player->SetPosition(player->GetCurrentNode());
		}

		// Check if each ghost collides with a wall
		for (std::vector<Ghost*>::iterator aiIter = ghostList.begin(); aiIter != ghostList.end(); ++aiIter)
		{
			if (CollisionChecker((*aiIter)->GetBoundingRect(), (*iter)->GetBoundingRect()))
			{
				//printf("A collision has occured between the AI and a wall!\n");
				int randVal = std::rand() % 4; // 4 is the number of moving states
				(*aiIter)->SetPosition((*aiIter)->GetCurrentNode());
				(*aiIter)->SetDirection((DirectionEnum)randVal);
			}
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
			score += 10;
			printf("Score: %d\n", score);
			// The break is necessary for two reasons
			// 1) we stop checking for nodes once we found we collided with one
			// 2) if we allow iter to increment we will crash because we are erasing elements
			break;
		}
	}

	// Check player collision against the AI
	for (std::vector<Ghost*>::iterator iter = ghostList.begin(); iter != ghostList.end(); ++iter)
	{
		if (CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
			
			if (player->Kill()) isLevelOver = true;
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

	// Render the AI
	for (std::vector<Ghost*>::iterator iter = ghostList.begin(); iter != ghostList.end(); ++iter)
	{
		(*iter)->Render(renderer);
	}

	// Display the score
	if (arialFont != NULL)
	{
		std::string scoreString = "Score: " + std::to_string(score);
		scoreFontSurface = TTF_RenderText_Solid(arialFont, scoreString.c_str(), SDL_Color{ 255, 255, 255 });
		scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreFontSurface);
		SDL_FreeSurface(scoreFontSurface);
		scoreFontSurface = NULL;
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreTextRect);
	}

	if (isLevelOver)
	{
		SDL_Rect endGameRect;
		endGameRect.w = G_SIZE * 10;
		endGameRect.h = G_SIZE * 2;
		endGameRect.x = SCREEN_WIDTH / 2 - endGameRect.w / 2;
		endGameRect.y = SCREEN_HEIGHT / 2 - endGameRect.h / 2;

		scoreFontSurface = TTF_RenderText_Solid(arialFont, endGameMessage.c_str(), SDL_Color{ 255, 255, 255 });
		scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreFontSurface);
		SDL_FreeSurface(scoreFontSurface);
		scoreFontSurface = NULL;
		SDL_RenderCopy(renderer, scoreTexture, NULL, &endGameRect);
	}

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