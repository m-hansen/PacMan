#include "GameplayScreen.h"

GameplayScreen GameplayScreen::gameplayScreen;

void GameplayScreen::Pause()
{
	isPaused = true;
	fprintf(stdout, "GameplayScreen paused\n");
}

void GameplayScreen::Resume()
{
	isPaused = false;
	fprintf(stdout, "GameplayScreen resumed\n");
}

void GameplayScreen::LoadContent(SDL_Renderer* renderer)
{
	// Load textures
	TextureManager::LoadTexture(renderer, "tile", "Resources/tile.png");
	TextureManager::LoadTexture(renderer, "pacmanRight", "Resources/pac-man_R.png");
	TextureManager::LoadTexture(renderer, "pacmanLeft", "Resources/pac-man_L.png");
	TextureManager::LoadTexture(renderer, "pacmanUp", "Resources/pac-man_U.png");
	TextureManager::LoadTexture(renderer, "pacmanDown", "Resources/pac-man_D.png");
	TextureManager::LoadTexture(renderer, "pacmanClosed", "Resources/pac-man_closed.png");
	TextureManager::LoadTexture(renderer, "wall", "Resources/wall.png");
	TextureManager::LoadTexture(renderer, "pellet", "Resources/pellet.png");
	TextureManager::LoadTexture(renderer, "blinky", "Resources/blinky.png");
	TextureManager::LoadTexture(renderer, "pinky", "Resources/pinky.png");
	TextureManager::LoadTexture(renderer, "inky", "Resources/inky.png");
	TextureManager::LoadTexture(renderer, "clyde", "Resources/clyde.png");
}

void GameplayScreen::Initialize(Game* game)
{
	// Load all content first
	LoadContent(game->renderer);
	arialFont = TTF_OpenFont("Resources/Fonts/ARIAL.TTF", G_SIZE);

	// Initialize variables
	score = 0;
	scoreTextRect.w = G_SIZE * 6;
	scoreTextRect.h = G_SIZE * 1.5;
	scoreTextRect.x = 4 * G_SIZE;
	scoreTextRect.y = 32 * G_SIZE;
	endGameMessage = "Game Over!";
	isLevelOver = false;
	isDebugging = false;
	isPaused = false;

	// Create an instance of a level manager
	levelManager = LevelManager::Instance();
	
	// Construct the level list
	std::vector<std::string> levelList;
	levelList.push_back("Level1.txt");
	levelList.push_back("PowerPelletLevel.txt");
	levelList.push_back("Level0.txt");
	levelManager->CreateLevelList("Resources/LevelData", levelList);

	// Initialize the level manager with the level list
	levelManager->InitializeLevel();
}

void GameplayScreen::Cleanup(Game* game)
{
	TTF_CloseFont(arialFont);
	arialFont = NULL;
}

GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::HandleEvents(Game* game)
{
	// Handle events on queue
	while (SDL_PollEvent(&currentEvent) != 0)
	{
		// User requests quit
		if (currentEvent.type == SDL_QUIT)
		{
			game->Quit();
		}
		// User presses a key
		else if (currentEvent.type == SDL_KEYDOWN)
		{
			switch (currentEvent.key.keysym.sym)
			{
			case SDLK_UP:
				if (levelManager->GetPlayer()->isAlignedWithTile && levelManager->GetPlayer()->GetDirection() != DirectionEnum::Up)
				{
					//levelManager->GetPlayer()->SetPosition(levelManager->GetPlayer()->GetCurrentNode());

					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Down) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Up) : levelManager->GetPlayer()->QueueDirection(DirectionEnum::Up);
				}
				break;
			case SDLK_DOWN:
				if (levelManager->GetPlayer()->isAlignedWithTile && levelManager->GetPlayer()->GetDirection() != DirectionEnum::Down)
				{
					//levelManager->GetPlayer()->SetPosition(levelManager->GetPlayer()->GetCurrentNode());
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Up) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Down) : levelManager->GetPlayer()->QueueDirection(DirectionEnum::Down);
				}
				break;
			case SDLK_LEFT:
				if (levelManager->GetPlayer()->isAlignedWithTile && levelManager->GetPlayer()->GetDirection() != DirectionEnum::Left)
				{
					//levelManager->GetPlayer()->SetPosition(levelManager->GetPlayer()->GetCurrentNode());
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Right) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Left) : levelManager->GetPlayer()->QueueDirection(DirectionEnum::Left);
				}
				break;
			case SDLK_RIGHT:
				if (levelManager->GetPlayer()->isAlignedWithTile && levelManager->GetPlayer()->GetDirection() != DirectionEnum::Right)
				{
					//levelManager->GetPlayer()->SetPosition(levelManager->GetPlayer()->GetCurrentNode());
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Left) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Right) : levelManager->GetPlayer()->QueueDirection(DirectionEnum::Right);
				}
				break;
			case SDLK_r:
				// Reset the game
				game->ChangeScreen(this);
			case SDLK_LEFTBRACKET:
				// Go to previous level
				levelManager->PreviousLevel();
				break;
			case SDLK_RIGHTBRACKET:
				// Go to next level
				levelManager->NextLevel();
				break;
			case SDLK_F3:
				// Toggle debugging information
				isDebugging = !isDebugging;
				break;
			case SDLK_ESCAPE:
			case SDLK_p:
				// Pause or resume the game
				(isPaused) ? Resume() : Pause();
				break;
			}
		}

		previousEvent = currentEvent;
	}
}

//void GameplayScreen::InitializeLevel(std::string lvlName)
//{
//	// Load the level, create the nodes, and determine the edges
//	levelManager->LoadLevel(lvlName.c_str());
//	level = new Graph();
//	level->GenerateGraph(SCREEN_WIDTH / G_SIZE, SCREEN_HEIGHT / G_SIZE);
//	levelManager->FindEdges();
//
//	// Initialize the player
//	player = new Player();
//	levelManager->GetPlayer()->Initialize();
//
//	// Load the AI
//	blinky = new Ghost("blinky", 12.0f, 5.0f, DirectionEnum::Left);
//	pinky = new Ghost("pinky", 15.0f, 5.0f, DirectionEnum::Right);
//	inky = new Ghost("inky", 9.0f, 5.0f, DirectionEnum::Down);
//	clyde = new Ghost("clyde", 18.0f, 5.0f, DirectionEnum::Down);
//
//	levelManager->GetGhosts().push_back(blinky);
//	levelManager->GetGhosts().push_back(pinky);
//	levelManager->GetGhosts().push_back(inky);
//	levelManager->GetGhosts().push_back(clyde);
//
//	score = 0;
//}

void GameplayScreen::Update(Game* game)
{
	// Immediately return if paused
	if (isPaused) 
		return;

	// Check if the player has run out of lives
	if (!levelManager->GetPlayer()->IsAlive())
	{
		isLevelOver = true;
	}

	// Check for victory condition
	if (levelManager->GetPellets().empty())
	{
		// Go to the next level if one exists
		if (levelManager->NextLevel() == false)
		{
			// We played through every level
			endGameMessage = "Congratulations!";
			isLevelOver = true;
		}
		return;
	}

	// Update the player
	levelManager->GetPlayer()->Update(game->GetDeltaTime());

	// Update the AI
	for (std::vector<Ghost*>::iterator iter = levelManager->GetGhosts().begin(); 
		iter != levelManager->GetGhosts().end(); ++iter)
	{
		(*iter)->Update(game->GetDeltaTime());
	}

	// Handle the collisions last
	HandleCollisions();
}

void GameplayScreen::Render(Game* game)
{
	// Clear color
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(game->renderer);

	// Render each node in the level
	if (isDebugging)
	{
		// Iterate over each node in the level graph
		//for (std::vector<Node*>::iterator iter = level->GetAllNodes()->begin(); iter != level->GetAllNodes()->end(); ++iter)
		for (std::vector<Node*>::iterator iter = levelManager->GetLegalNodes().begin();
			iter != levelManager->GetLegalNodes().end(); ++iter)
		{
			Node* node = (*iter);
			Vector2f loc = node->GetLocation();

			// Temporarily set the rendering color to white for the nodes
			SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // white
			switch (node->GetType())
			{
				//case NodeType::Empty:
				//SDL_RenderCopy(renderer, TextureManager::GetTexture(, NULL, &boundingRect);
			}
			SDL_RenderDrawPoint(game->renderer, loc.x * G_SIZE, loc.y * G_SIZE);

			// Draw each node's bounding rectangle
			(*iter)->Render(game->renderer);

			SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // black
		}
	}

	// Render the walls
	for (std::vector<Wall*>::iterator iter = levelManager->GetWalls().begin();
		iter != levelManager->GetWalls().end(); ++iter)
	{
		(*iter)->Render(game->renderer);
	}

	// Render each remaining pellet on the board
	for (std::vector<Pellet*>::iterator iter = levelManager->GetPellets().begin();
		iter != levelManager->GetPellets().end(); ++iter)
	{
		(*iter)->Render(game->renderer);
	}

	// Render the player
	levelManager->GetPlayer()->Render(game->renderer);

	// Render the AI
	for (std::vector<Ghost*>::iterator iter = levelManager->GetGhosts().begin(); 
		iter != levelManager->GetGhosts().end(); ++iter)
	{
		(*iter)->Render(game->renderer);
	}

	// Display the score
	if (arialFont != NULL)
	{
		std::string scoreString = "Score: " + std::to_string(score);
		scoreFontSurface = TTF_RenderText_Solid(arialFont, scoreString.c_str(), SDL_Color{ 255, 255, 255 });
		scoreTexture = SDL_CreateTextureFromSurface(game->renderer, scoreFontSurface);
		SDL_FreeSurface(scoreFontSurface);
		scoreFontSurface = NULL;
		SDL_RenderCopy(game->renderer, scoreTexture, NULL, &scoreTextRect);
	}

	if (isPaused)
	{
		SDL_Rect pauseRect;
		pauseRect.w = G_SIZE * 10;
		pauseRect.h = G_SIZE * 2;
		pauseRect.x = SCREEN_WIDTH / 2 - pauseRect.w / 2;
		pauseRect.y = SCREEN_HEIGHT / 2 - pauseRect.h / 2;

		SDL_Surface* fontSurface = TTF_RenderText_Solid(arialFont, "PAUSED", SDL_Color{ 255, 255, 255 });
		SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(game->renderer, fontSurface);
		SDL_FreeSurface(fontSurface);
		fontSurface = NULL;
		SDL_RenderCopy(game->renderer, fontTexture, NULL, &pauseRect);
	}

	if (isLevelOver)
	{
		SDL_Rect endGameRect;
		endGameRect.w = G_SIZE * 10;
		endGameRect.h = G_SIZE * 2;
		endGameRect.x = SCREEN_WIDTH / 2 - endGameRect.w / 2;
		endGameRect.y = SCREEN_HEIGHT / 2 - endGameRect.h / 2;

		scoreFontSurface = TTF_RenderText_Solid(arialFont, endGameMessage.c_str(), SDL_Color{ 255, 255, 255 });
		scoreTexture = SDL_CreateTextureFromSurface(game->renderer, scoreFontSurface);
		SDL_FreeSurface(scoreFontSurface);
		scoreFontSurface = NULL;
		SDL_RenderCopy(game->renderer, scoreTexture, NULL, &endGameRect);
	}

	// Update the screen
	SDL_RenderPresent(game->renderer);
}

void GameplayScreen::HandleCollisions()
{
	// Check which node the player/AI occupies
	for (std::vector<Node*>::iterator iter = levelManager->GetLegalNodes().begin();
		iter != levelManager->GetLegalNodes().end(); ++iter)
	{
		// Check if player is inside a node
		if (Utils::CollisionChecker(levelManager->GetPlayer()->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
			levelManager->GetPlayer()->UpdateNodes(*iter);
			if (levelManager->GetPlayer()->GetCurrentNode() != levelManager->GetPlayer()->GetPreviousNode())
			{
				// Print the node ID that the player enters
				printf("Player is in node ID : %d\n", (*iter)->GetNodeId());
				levelManager->GetPlayer()->SetPreviousDirection(levelManager->GetPlayer()->GetDirection());
			}

		}

		// Iterate over each ghost
		for (std::vector<Ghost*>::iterator aiIter = levelManager->GetGhosts().begin(); aiIter != levelManager->GetGhosts().end(); ++aiIter)
		{
			// Check if a ghost is inside a node
			if (Utils::CollisionChecker((*aiIter)->GetBoundingRect(), (*iter)->GetBoundingRect()))
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
	for (std::vector<Wall*>::iterator iter = levelManager->GetWalls().begin(); iter != levelManager->GetWalls().end(); ++iter)
	{
		// Check if the player collides with a wall
		if (Utils::CollisionChecker(levelManager->GetPlayer()->GetSpriteRect(), (*iter)->GetBoundingRect()))
		{
			if (((levelManager->GetPlayer()->GetDirection() == DirectionEnum::Up) ||
				(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Down))
				&&
				((levelManager->GetPlayer()->GetPreviousDirection() == DirectionEnum::Left) ||
				(levelManager->GetPlayer()->GetPreviousDirection() == DirectionEnum::Right)))
			{
				levelManager->GetPlayer()->SetDirection(levelManager->GetPlayer()->GetPreviousDirection());
			}
			else if (((levelManager->GetPlayer()->GetDirection() == DirectionEnum::Left) ||
				(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Right))
				&&
				((levelManager->GetPlayer()->GetPreviousDirection() == DirectionEnum::Up) ||
				(levelManager->GetPlayer()->GetPreviousDirection() == DirectionEnum::Down)))
			{
				levelManager->GetPlayer()->SetDirection(levelManager->GetPlayer()->GetPreviousDirection());
			}
			else
			{
				levelManager->GetPlayer()->SetDirection(DirectionEnum::None);

			}
			levelManager->GetPlayer()->SetPosition(levelManager->GetPlayer()->GetCurrentNode());
		}

		// Check if each ghost collides with a wall
		/*for (std::vector<Ghost*>::iterator aiIter = levelManager->GetGhosts().begin(); aiIter != levelManager->GetGhosts().end(); ++aiIter)
		{
		if (Utils::CollisionChecker((*aiIter)->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
		//printf("A collision has occured between the AI and a wall!\n");
		int randVal = std::rand() % 4; // 4 is the number of moving states
		(*aiIter)->SetPosition((*aiIter)->GetCurrentNode());
		(*aiIter)->SetDirection((DirectionEnum)randVal);
		}
		}*/
	}

	// Check for collisions between player and consumable
	// NOTE: we must use iter++ instead of ++iter
	for (std::vector<Pellet*>::iterator iter = levelManager->GetPellets().begin(); iter != levelManager->GetPellets().end(); iter++)
	{
		if (Utils::CollisionChecker(levelManager->GetPlayer()->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{
			if ((*iter)->GetType() == NodeTypeEnum::PowerPelletNode)
			{
				// Reverse the ghosts directions
				// TODO this will be changed after pathfinding in place
				for (std::vector<Ghost*>::iterator iter = levelManager->GetGhosts().begin();
					iter != levelManager->GetGhosts().end(); ++iter)
				{
					(*iter)->ReverseDirection();
				}
			}

			// Increment the score
			score += (*iter)->GetValue();

			// Free memory, cleat the list, and break
			delete (*iter);
			levelManager->GetPellets().erase(iter);

			// The break is necessary for two reasons
			// 1) we stop checking for nodes once we found we collided with one
			// 2) if we allow iter to increment we will crash because we are erasing elements
			break;
		}
	}

	// Check player collision against the AI
	if (levelManager->GetPlayer()->IsAlive())
	{
		for (std::vector<Ghost*>::iterator iter = levelManager->GetGhosts().begin();
			iter != levelManager->GetGhosts().end(); ++iter)
		{
			if (Utils::CollisionChecker(levelManager->GetPlayer()->GetBoundingRect(),
				(*iter)->GetBoundingRect()))
			{
				levelManager->GetPlayer()->LoseLife();
				levelManager->ResetAI();
			}
		}
	}

}