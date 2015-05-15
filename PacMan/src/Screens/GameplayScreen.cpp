#include "Screens/GameplayScreen.h"

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
	TextureManager::LoadTexture(renderer, "playerLives", "Resources/player-lives.png");
	TextureManager::LoadTexture(renderer, "wall", "Resources/wall.png");
	TextureManager::LoadTexture(renderer, "wall_top_left", "Resources/wall_top_left_corner.png");
	TextureManager::LoadTexture(renderer, "wall_top_right", "Resources/wall_top_right_corner.png");
	TextureManager::LoadTexture(renderer, "wall_bottom_left", "Resources/wall_bottom_left_corner.png");
	TextureManager::LoadTexture(renderer, "wall_bottom_right", "Resources/wall_bottom_right_corner.png");
	TextureManager::LoadTexture(renderer, "wall_horizontal", "Resources/wall_horizontal.png");
	TextureManager::LoadTexture(renderer, "wall_vertical", "Resources/wall_vertical.png");
	TextureManager::LoadTexture(renderer, "pellet", "Resources/pellet.png");
	TextureManager::LoadTexture(renderer, "powerPellet", "Resources/power-pellet.png");
	TextureManager::LoadTexture(renderer, "redEnemy", "Resources/red-enemy.png");
	TextureManager::LoadTexture(renderer, "pinkEnemy", "Resources/pink-enemy.png");
	TextureManager::LoadTexture(renderer, "blueEnemy", "Resources/blue-enemy.png");
	TextureManager::LoadTexture(renderer, "orangeEnemy", "Resources/orange-enemy.png");
	TextureManager::LoadTexture(renderer, "frightened", "Resources/frightened-ghost.png");
	TextureManager::LoadTexture(renderer, "frightenedWhite", "Resources/frightened-white.png");
	// Load in sprite sheets
	TextureManager::LoadTexture(renderer, "player", "Resources/player-sprite-sheet.png");
	TextureManager::LoadTexture(renderer, "walls", "Resources/wall-sprite-sheet.png");

	// Load audio
	Utils::LoadMusic(&background, "Resources/Audio/background.ogg");
	Utils::LoadMusic(&backgroundPower, "Resources/Audio/background-power.ogg");
	Utils::LoadMixChunk(&eatEnemy, "Resources/Audio/eat-enemy.ogg");
	Utils::LoadMixChunk(&pop, "Resources/Audio/pop.ogg");
	Utils::LoadMixChunk(&powerUp, "Resources/Audio/power-up.ogg");
	Utils::LoadMixChunk(&death, "Resources/Audio/death.ogg");
}

void GameplayScreen::Initialize(Game* game)
{
	// Load all content first
	LoadContent(game->renderer);
	arialFont = TTF_OpenFont("Resources/Fonts/ARIAL.TTF", Config::gridSize);

	SDL_SetTextureColorMod(TextureManager::GetTexture("wall"), 
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_top_left"), 
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_top_right"), 
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_bottom_left"),
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_bottom_right"), 
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_horizontal"), 
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_vertical"), 
		Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);

	nodeDisplayFlags[ID] = false;
	nodeDisplayFlags[G] = false;
	nodeDisplayFlags[H] = false;
	nodeDisplayFlags[F] = false;

	// Initialize variables
	score = 0;
	scoreTextRect.w = Config::gridSize * 6;
	scoreTextRect.h = Config::gridSize * 1.5;
	scoreTextRect.x = 4 * Config::gridSize;
	scoreTextRect.y = 32 * Config::gridSize;
	endGameMessage = "Game Over!";
	isLevelOver = false;
	isDebugging = false;
	isPaused = false;
	isGameOver = false;
	isPowerUpState = false;
	livesTexture = TextureManager::GetTexture("playerLives");
	livesRemaining = 2;
	// livesLeftRect.x intentionally not set here
	livesLeftRect.y = Config::gridSize * 32; // 32 is the vertical node offset
	livesLeftRect.w = Config::gridSize;
	livesLeftRect.h = Config::gridSize;

	SDL_Surface* pauseSurface = TTF_RenderText_Solid(arialFont, "PAUSED", SDL_Color{ 255, 255, 255 });
	pauseTexture = SDL_CreateTextureFromSurface(game->renderer, pauseSurface);
	SDL_FreeSurface(pauseSurface);
	pauseSurface = NULL;

	// Create an instance of a level manager
	levelManager = LevelManager::Instance();
	
	// Construct the level list
	std::vector<std::string> levelList;
	levelList.push_back("Level1-New.txt");
	levelList.push_back("Level1Quick.txt");
	levelList.push_back("Level1.txt");
	levelList.push_back("PowerPelletLevel.txt");
	levelList.push_back("Level0.txt");
	levelManager->CreateLevelList("Resources/LevelData", levelList);

	// Initialize the level manager with the level list
	levelManager->InitializeLevel();

	// Initialize the pathfinder with the legal playing nodes
	//pathfinder = new Pathfinder(levelManager->GetLegalNodes());

	// Adjust the sound based on settings
	Mix_VolumeMusic(MIX_MAX_VOLUME * Config::musicVol);
	Mix_VolumeChunk(eatEnemy, MIX_MAX_VOLUME * Config::sfxVol);
	Mix_VolumeChunk(pop, MIX_MAX_VOLUME * Config::sfxVol);
	Mix_VolumeChunk(powerUp, MIX_MAX_VOLUME * Config::sfxVol);
	Mix_VolumeChunk(death, MIX_MAX_VOLUME * Config::sfxVol);

	// Play the background music
	Mix_PlayMusic(background, -1); // -1 means loop until stopped
}

void GameplayScreen::Cleanup(Game* game)
{
	levelManager->CleanupLevel();

	SDL_DestroyTexture(pauseTexture);
	pauseTexture = NULL;

	TTF_CloseFont(arialFont);
	arialFont = NULL;

	// Unload the textures that were loaded from the GameplayScreen
	TextureManager::UnloadTexture("tile");
	TextureManager::UnloadTexture("playerLives");
	TextureManager::UnloadTexture("wall");
	TextureManager::UnloadTexture("wall_top_left");
	TextureManager::UnloadTexture("wall_top_right");
	TextureManager::UnloadTexture("wall_bottom_left");
	TextureManager::UnloadTexture("wall_bottom_right");
	TextureManager::UnloadTexture("wall_horizontal");
	TextureManager::UnloadTexture("wall_vertical");
	TextureManager::UnloadTexture("pellet");
	TextureManager::UnloadTexture("powerPellet");
	TextureManager::UnloadTexture("redEnemy");
	TextureManager::UnloadTexture("pinkEnemy");
	TextureManager::UnloadTexture("blueEnemy");
	TextureManager::UnloadTexture("orangeEnemy");
	TextureManager::UnloadTexture("frightened");
	TextureManager::UnloadTexture("frightenedWhite");
	TextureManager::UnloadTexture("player");
	TextureManager::UnloadTexture("walls");

	// Free the sound effects
	Mix_FreeMusic(background);
	Mix_FreeMusic(backgroundPower);
	Mix_FreeChunk(eatEnemy);
	Mix_FreeChunk(pop);
	Mix_FreeChunk(powerUp);
	Mix_FreeChunk(death);
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
		else if (currentEvent.type == SDL_KEYDOWN
			|| currentEvent.type == SDL_JOYBUTTONDOWN
			|| currentEvent.type == SDL_JOYAXISMOTION)
		{
			if (currentEvent.key.keysym.sym == SDLK_UP
				|| (currentEvent.jaxis.which == 0
				&& currentEvent.jaxis.axis == 1
				&& currentEvent.jaxis.value < -Config::joystickDeadZone))
			{
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile &&
					levelManager->GetPlayer()->GetDirection() != DirectionEnum::Up)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Down) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Up) :
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Up);
				}
			}
			else if (currentEvent.key.keysym.sym == SDLK_DOWN
				|| (currentEvent.jaxis.which == 0
				&& currentEvent.jaxis.axis == 1
				&& currentEvent.jaxis.value > Config::joystickDeadZone))
			{
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile
					&& levelManager->GetPlayer()->GetDirection() != DirectionEnum::Down)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Up) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Down) :
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Down);
				}
			}
			else if (currentEvent.key.keysym.sym == SDLK_LEFT
				|| (currentEvent.jaxis.which == 0
				&& currentEvent.jaxis.axis == 0
				&& currentEvent.jaxis.value < -Config::joystickDeadZone))
			{
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile &&
					levelManager->GetPlayer()->GetDirection() != DirectionEnum::Left)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Right) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Left) :
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Left);
				}
			}
			else if (currentEvent.key.keysym.sym == SDLK_RIGHT
				|| (currentEvent.jaxis.which == 0
				&& currentEvent.jaxis.axis == 0
				&& currentEvent.jaxis.value > Config::joystickDeadZone))
			{
				if (!isPaused && levelManager->GetPlayer()->isAlignedWithTile
					&& levelManager->GetPlayer()->GetDirection() != DirectionEnum::Right)
				{
					// Quick movement if we are moving along the same axis
					// queued movement otherwise
					(levelManager->GetPlayer()->GetDirection() == DirectionEnum::Left) ?
						levelManager->GetPlayer()->SetDirection(DirectionEnum::Right) :
						levelManager->GetPlayer()->QueueDirection(DirectionEnum::Right);
				}
			}
			else if (currentEvent.key.keysym.sym == SDLK_r)
			{
				// Reset the game
				game->ChangeScreen(this);
			}
			else if (currentEvent.key.keysym.sym == SDLK_LEFTBRACKET)
			{
				// Go to previous level
				levelManager->PreviousLevel();
			}
			else if (currentEvent.key.keysym.sym == SDLK_RIGHTBRACKET)
			{
				// Go to next level
				levelManager->NextLevel();
			}
			else if (currentEvent.key.keysym.sym == SDLK_F3)
			{
				// Toggle debugging information
				isDebugging = !isDebugging;
			}
			else if (currentEvent.key.keysym.sym == SDLK_0)
			{
				nodeDisplayFlags[ID] = !nodeDisplayFlags[ID];
			}
			else if (currentEvent.key.keysym.sym == SDLK_1)
			{
				nodeDisplayFlags[G] = !nodeDisplayFlags[G];
			}
			else if (currentEvent.key.keysym.sym == SDLK_2)
			{
				nodeDisplayFlags[H] = !nodeDisplayFlags[H];
			}
			else if (currentEvent.key.keysym.sym == SDLK_3)
			{
				nodeDisplayFlags[F] = !nodeDisplayFlags[F];
			}
			else if (currentEvent.cbutton.button == 4 // Start button
				|| currentEvent.key.keysym.sym == SDLK_ESCAPE
				|| currentEvent.key.keysym.sym == SDLK_SPACE
				|| currentEvent.key.keysym.sym == SDLK_RETURN
				|| currentEvent.key.keysym.sym == SDLK_p)
			{
				if (!isLevelOver)
				{
					// Pause or resume the game
					(isPaused) ? Resume() : Pause();
				}
			}
		}

		previousEvent = currentEvent;
	}
}

void GameplayScreen::Update(Game* game)
{
	// Immediately return if paused
	if (isPaused || isGameOver)
		return;

	if (isPowerUpState)
	{
		if (Ghost::CurrentState() != Frightened)
		{
			// Return to standard background music
			isPowerUpState = false;
			Mix_PlayMusic(background, -1);
		}
	}

	if (levelManager->GetPlayer()->HasDeathAnimationFinished())
	{
		// Check if we are out of lives
		if (livesRemaining <= 0)
		{
			fprintf(stdout, "Game Over!\n");
			GameEnd(0);
			return;
		}
		else
		{
			levelManager->ResetAgentPositions();
		}
	}

	// Check for victory condition
	if (levelManager->GetPellets().empty())
	{
		// The level was completed
		if (levelManager->LevelCompletedAnimation())
		{
			// Go to the next level if one exists
			if (levelManager->NextLevel() == false)
				GameEnd(1);
		}
		return;
	}

	levelManager->Update(game->GetDeltaTime());

	//// Update the player
	//levelManager->GetPlayer()->Update(game->GetDeltaTime());

	//// Update the AI
	//for (std::vector<Ghost*>::iterator iter = levelManager->GetGhosts().begin(); 
	//	iter != levelManager->GetGhosts().end(); ++iter)
	//{
	//	(*iter)->Update(game->GetDeltaTime());
	//}

	// Handle the collisions last
	HandleCollisions();
}

void GameplayScreen::Render(Game* game)
{
	// Clear color
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(game->renderer);

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

	// Render the walls
	for (std::vector<Sprite*>::iterator iter = levelManager->GetWalls().begin();
		iter != levelManager->GetWalls().end(); ++iter)
	{
		(*iter)->Render(game->renderer);
	}

	// Render the GUI elements
	RenderGUI(game->renderer);

	// Draw additional information for debugging
	if (isDebugging)
	{
		DrawDebug(game->renderer);
	}

	// Update the screen
	SDL_RenderPresent(game->renderer);
}

void GameplayScreen::RenderGUI(SDL_Renderer* renderer)
{
	// Display the lives left
	for (int i = 0; i < livesRemaining - 1; i++)
	{
		livesLeftRect.x = Config::gridSize * i;
		SDL_RenderCopy(renderer, livesTexture, NULL, &livesLeftRect);
	}

	// Display the score
	if (arialFont != NULL)
	{
		std::string scoreString = "Score: " + std::to_string(score);
		Utils::RenderText(renderer, arialFont, scoreString, SDL_Color{ 255, 255, 255 }, &scoreTextRect);
	}

	// Display the AI state
	std::string aiStateString = Ghost::CurrentStateName();
	SDL_Rect aiStateRect;
	aiStateRect.x = Config::gridSize * 18; 
	aiStateRect.y = Config::gridSize * 32; 
	aiStateRect.w = Config::gridSize * 8; 
	aiStateRect.h = Config::gridSize * 2;
	Utils::RenderText(renderer, arialFont, "AI State: " + aiStateString, SDL_Color{ 255, 255, 255 }, &aiStateRect);

	if (isPaused)
	{
		SDL_Rect pauseRect;
		pauseRect.w = Config::gridSize * 10;
		pauseRect.h = Config::gridSize * 2;
		pauseRect.x = Config::screenWidth / 2 - pauseRect.w / 2;
		pauseRect.y = Config::screenHeight / 2 - pauseRect.h / 2;

		SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
	}

	if (isLevelOver)
	{
		const int VERT_OFFSET = 7;
		SDL_Rect endGameRect;
		endGameRect.w = Config::gridSize * 6;
		endGameRect.h = Config::gridSize * 1.2;
		endGameRect.x = Config::screenWidth / 2 - endGameRect.w / 2;
		endGameRect.y = Config::screenHeight / 2 - endGameRect.h / 2 - VERT_OFFSET;

		TTF_SetFontStyle(arialFont, TTF_STYLE_BOLD);
		Utils::RenderText(renderer, arialFont, endGameMessage, SDL_Color{ 255, 255, 255 }, &endGameRect);
		TTF_SetFontStyle(arialFont, TTF_STYLE_NORMAL);
	}
}

void GameplayScreen::DrawDebug(SDL_Renderer* renderer)
{
	// Iterate over each node in the level graph
	for (std::vector<Node*>::iterator iter = levelManager->GetLegalNodes().begin();
		iter != levelManager->GetLegalNodes().end(); ++iter)
	{
		Node* node = (*iter);
		Vector2f loc = node->GetPosition();

		// Temporarily set the rendering color to white for the nodes
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
		switch (node->GetType())
		{
			//case NodeType::Empty:
			//SDL_RenderCopy(renderer, TextureManager::GetTexture(, NULL, &boundingRect);
		}
		SDL_RenderDrawPoint(renderer, loc.x, loc.y);

		// Draw each node's bounding rectangle
		(*iter)->Render(renderer);

		SDL_Texture* nodeIdText = NULL;

		// Display the node id
		if (nodeDisplayFlags[ID])
		{
			Utils::RenderText(renderer, arialFont, std::to_string((*iter)->GetNodeId()),
				SDL_Color{ 255, 255, 255 }, (*iter)->GetBoundingRect());
		}

		// Display the G val
		if (nodeDisplayFlags[G])
		{
			Utils::RenderText(renderer, arialFont, std::to_string((*iter)->GetMovementCost()),
				SDL_Color{ 255, 255, 255 }, (*iter)->GetBoundingRect());
		}

		// Display the H val
		if (nodeDisplayFlags[H])
		{
			Utils::RenderText(renderer, arialFont, std::to_string((*iter)->GetHeuristic()),
				SDL_Color{ 255, 255, 255 }, (*iter)->GetBoundingRect());
		}

		// Display the F val
		if (nodeDisplayFlags[F])
		{
			Utils::RenderText(renderer, arialFont, std::to_string((*iter)->GetTotalCost()),
				SDL_Color{ 255, 255, 255 }, (*iter)->GetBoundingRect());
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	}

	// Display the current node ID for each ghost
	auto ghostList = levelManager->GetGhosts();
	for (int i = 0; i < ghostList.size(); i++)
	{
		if (ghostList[i]->GetCurrentNode() != NULL)
		{
			const int OFFSET = 20;
			SDL_Rect loc;
			loc.x = 100;
			loc.y = 200 + (i * OFFSET);
			loc.w = 40;
			loc.h = 30;
			Utils::RenderText(renderer, arialFont, std::to_string(ghostList[i]->GetCurrentNode()->GetNodeId()),
				SDL_Color{ 255, 255, 255 }, &loc);
		}
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
					Mix_PlayMusic(backgroundPower, -1);
					isPowerUpState = true;
				}
			}

			// Increment the score
			score += (*iter)->GetValue();

			// Play the appropriate sound clip
			if ((*iter)->GetType() == PelletNode)
			{
				Mix_PlayChannel(-1, pop, 0);
			}
			else if ((*iter)->GetType() == PowerPelletNode)
			{
				Mix_PlayChannel(-1, powerUp, 0);
			}

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
					Mix_PlayChannel(-1, eatEnemy, 0);
				}
				else
				{
					PlayerDeath();
				}
			}
		}
	}
}

void GameplayScreen::PlayerDeath()
{
	// Player is killed
	levelManager->GetPlayer()->Kill();
	Mix_PlayChannel(-1, death, 0);

	// Hide AI while player dies
	for (int i = 0; i < levelManager->GetGhosts().size(); i++)
	{
		levelManager->GetGhosts()[i]->SetVisible(false);
	}

	livesRemaining--;
}

void GameplayScreen::GameEnd(int condition)
{
	isLevelOver = true;
	isGameOver = true;

	if (condition == 1)
	{
		// Player wins!
		// We completed every level
		endGameMessage = "Congratulations!";
	}
	else
	{
		// Intentionally left blank
		// Player died before completing the game
	}

	// Display the high score table
	HighScoreTable table;
	table.LoadHighScores(Config::highScoresFile);

	if (table.UploadScore("GenericName", score))
	{
		printf("Uploading your high score! Congratulations!\n");
		table.SaveHighScores(Config::highScoresFile);
	}

	// Log the high score table
	table.PrintHighScores();
}