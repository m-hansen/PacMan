#include "GameplayScreen.h"

GameplayScreen GameplayScreen::gameplayScreen;

void GameplayScreen::Pause()
{
	fprintf(stdout, "GameplayScreen paused\n");
	isPaused = true;
	Ghost::GetStateTimer().Pause();
}

void GameplayScreen::Resume()
{
	fprintf(stdout, "GameplayScreen resumed\n");
	isPaused = false;
	Ghost::GetStateTimer().Resume();
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
	TextureManager::LoadTexture(renderer, "powerPellet", "Resources/power-pellet.png");
	TextureManager::LoadTexture(renderer, "blinky", "Resources/blinky.png");
	TextureManager::LoadTexture(renderer, "pinky", "Resources/pinky.png");
	TextureManager::LoadTexture(renderer, "inky", "Resources/inky.png");
	TextureManager::LoadTexture(renderer, "clyde", "Resources/clyde.png");
	TextureManager::LoadTexture(renderer, "frightened", "Resources/frightened-ghost.png");
}

void GameplayScreen::Initialize(Game* game)
{
	// Load all content first
	LoadContent(game->renderer);
	arialFont = TTF_OpenFont("Resources/Fonts/ARIAL.TTF", GRID_SIZE);

	// Initialize variables
	score = 0;
	scoreTextRect.w = GRID_SIZE * 6;
	scoreTextRect.h = GRID_SIZE * 1.5;
	scoreTextRect.x = 4 * GRID_SIZE;
	scoreTextRect.y = 32 * GRID_SIZE;
	endGameMessage = "Game Over!";
	isLevelOver = false;
	isDebugging = false;
	isPaused = false;
	livesTexture = TextureManager::GetTexture("pacmanLeft");
	livesRemaining = 3;
	// livesLeftRect.x intentionally not set here
	livesLeftRect.y = GRID_SIZE * 32; // 32 is the vertical node offset
	livesLeftRect.w = GRID_SIZE;
	livesLeftRect.h = GRID_SIZE;

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
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile && 
					levelManager->GetPlayer()->GetDirection() != DirectionEnum::Up)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Down) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Up) : 
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Up);
				}
				break;
			case SDLK_DOWN:
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile
					&& levelManager->GetPlayer()->GetDirection() != DirectionEnum::Down)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Up) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Down) : 
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Down);
				}
				break;
			case SDLK_LEFT:
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile &&
					levelManager->GetPlayer()->GetDirection() != DirectionEnum::Left)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Right) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Left) : 
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Left);
				}
				break;
			case SDLK_RIGHT:
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile
					&& levelManager->GetPlayer()->GetDirection() != DirectionEnum::Right)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Left) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Right) : 
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Right);
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
			case SDLK_SPACE:
			case SDLK_RETURN:
			case SDLK_p:
				if (!isLevelOver)
				{
					// Pause or resume the game
					(isPaused) ? Resume() : Pause();
				}
				break;
			}
		}

		previousEvent = currentEvent;
	}
}

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
		for (std::vector<Node*>::iterator iter = levelManager->GetLegalNodes().begin();
			iter != levelManager->GetLegalNodes().end(); ++iter)
		{
			Node* node = (*iter);
			Vector2f loc = node->GetPosition();

			// Temporarily set the rendering color to white for the nodes
			SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // white
			switch (node->GetType())
			{
				//case NodeType::Empty:
				//SDL_RenderCopy(renderer, TextureManager::GetTexture(, NULL, &boundingRect);
			}
			SDL_RenderDrawPoint(game->renderer, loc.x, loc.y);

			// Draw each node's bounding rectangle
			(*iter)->Render(game->renderer);

			// Display the node id
			/*SDL_Texture* nodeIdText = Utils::CreateFontTexture(game->renderer, arialFont,
				std::to_string((*iter)->GetNodeId()), SDL_Color{ 255, 255, 255 });
			SDL_RenderCopy(game->renderer, nodeIdText, NULL, (*iter)->GetBoundingRect());
			SDL_DestroyTexture(nodeIdText);
			nodeIdText = NULL;*/

			SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // black
		}
	}

	// Render the walls
	for (std::vector<Sprite*>::iterator iter = levelManager->GetWalls().begin();
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

	// Render the GUI elements
	RenderGUI(game->renderer);

	// Update the screen
	SDL_RenderPresent(game->renderer);
}

void GameplayScreen::RenderGUI(SDL_Renderer* renderer)
{
	// Display the lives left
	for (int i = 0; i < livesRemaining - 1; i++)
	{
		livesLeftRect.x = GRID_SIZE * i;
		SDL_RenderCopy(renderer, livesTexture, NULL, &livesLeftRect);
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
		SDL_DestroyTexture(scoreTexture);
		scoreTexture = NULL;
	}

	// Display the AI state
	std::string aiStateString = Ghost::CurrentStateName();
	SDL_Surface* aiStateSurface = TTF_RenderText_Solid(arialFont, ("AI State: " + aiStateString).c_str(), SDL_Color{ 255, 255, 255 });
	SDL_Texture* aiStateTexture = SDL_CreateTextureFromSurface(renderer, aiStateSurface);
	SDL_FreeSurface(scoreFontSurface);
	scoreFontSurface = NULL;
	SDL_Rect aiStateRect;
	aiStateRect.x = GRID_SIZE * 18; aiStateRect.y = GRID_SIZE * 32; aiStateRect.w = GRID_SIZE * 8; aiStateRect.h = GRID_SIZE * 2;
	SDL_RenderCopy(renderer, aiStateTexture, NULL, &aiStateRect);
	SDL_DestroyTexture(aiStateTexture);
	aiStateTexture = NULL;

	if (isPaused)
	{
		SDL_Rect pauseRect;
		pauseRect.w = GRID_SIZE * 10;
		pauseRect.h = GRID_SIZE * 2;
		pauseRect.x = SCREEN_WIDTH / 2 - pauseRect.w / 2;
		pauseRect.y = SCREEN_HEIGHT / 2 - pauseRect.h / 2;

		SDL_Surface* fontSurface = TTF_RenderText_Solid(arialFont, "PAUSED", SDL_Color{ 255, 255, 255 });
		SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
		SDL_FreeSurface(fontSurface);
		fontSurface = NULL;
		SDL_RenderCopy(renderer, fontTexture, NULL, &pauseRect);
	}

	if (isLevelOver)
	{
		SDL_Rect endGameRect;
		endGameRect.w = GRID_SIZE * 10;
		endGameRect.h = GRID_SIZE * 2;
		endGameRect.x = SCREEN_WIDTH / 2 - endGameRect.w / 2;
		endGameRect.y = SCREEN_HEIGHT / 2 - endGameRect.h / 2;

		scoreFontSurface = TTF_RenderText_Solid(arialFont, endGameMessage.c_str(), SDL_Color{ 255, 255, 255 });
		scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreFontSurface);
		SDL_FreeSurface(scoreFontSurface);
		scoreFontSurface = NULL;
		SDL_RenderCopy(renderer, scoreTexture, NULL, &endGameRect);
	}
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
	for (std::vector<Sprite*>::iterator iter = levelManager->GetWalls().begin(); iter != levelManager->GetWalls().end(); ++iter)
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
				for (std::vector<Ghost*>::iterator iter = levelManager->GetGhosts().begin();
					iter != levelManager->GetGhosts().end(); ++iter)
				{
					(*iter)->ReverseDirection();
					(*iter)->EnterFrightenedState(0.55f);
					Ghost::ChangeState(GhostStateEnum::Frightened);
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
				if ((*iter)->IsFrightened())
				{
					// Eat the ghost
					(*iter)->Respawn();
					score += 500; // TODO adjust this 
				}
				else
				{
					// Player is killed

					// TODO play death animation
					livesRemaining--;

					// Check if we are out of lives
					if (livesRemaining <= 0)
					{
						fprintf(stdout, "Game Over!\n");
						levelManager->GetPlayer()->Kill();
					}
					else
					{
						levelManager->ResetAgentPositions();
					}
				}
			}
		}
	}

}