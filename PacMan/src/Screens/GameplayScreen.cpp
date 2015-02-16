#include "GameplayScreen.h"

GameplayScreen GameplayScreen::gameplayScreen;

void GameplayScreen::Initialize(Game* game)
{
	level = NULL;
	score = 0;
	arialFont = TTF_OpenFont("Resources\\Fonts\\ARIAL.TTF", G_SIZE);
	scoreTextRect.w = G_SIZE * 6;
	scoreTextRect.h = G_SIZE * 1.5;
	scoreTextRect.x = 4 * G_SIZE;
	scoreTextRect.y = 32 * G_SIZE;
	endGameMessage = "Game Over!";
	isLevelOver = false;
	isDebugging = false;
	InitializeLevel("Level1.txt");
}

void GameplayScreen::Cleanup(Game* game)
{
	delete (blinky);
	blinky = NULL;
	delete (pinky);
	pinky = NULL;
	delete (inky);
	inky = NULL;
	delete (clyde);
	clyde = NULL;

	delete (player);
	player = NULL;

	delete (level);
	level = NULL;

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
				if (player->isAlignedWithTile && player->GetDirection() != DirectionEnum::Up)
				{
					player->SetPosition(player->GetCurrentNode());
					player->SetDirection(Up);
				}
				break;
			case SDLK_DOWN:
				if (player->isAlignedWithTile && player->GetDirection() != DirectionEnum::Down)
				{
					player->SetPosition(player->GetCurrentNode());
					player->SetDirection(Down);
				}
				break;
			case SDLK_LEFT:
				if (player->isAlignedWithTile && player->GetDirection() != DirectionEnum::Left)
				{
					player->SetPosition(player->GetCurrentNode());
					player->SetDirection(Left);
				}
				break;
			case SDLK_RIGHT:
				if (player->isAlignedWithTile && player->GetDirection() != DirectionEnum::Right)
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
				game->Quit();
				break;
			}
		}

		previousEvent = currentEvent;
	}
}

void GameplayScreen::InitializeLevel(std::string lvlName)
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
	blinky = new Ghost("blinky", 12, 15);
	pinky = new Ghost("pinky", 15, 15);
	inky = new Ghost("inky", 12, 15);
	clyde = new Ghost("clyde", 12, 15);

	ghostList.push_back(blinky);
	ghostList.push_back(pinky);
	ghostList.push_back(inky);
	ghostList.push_back(clyde);

	score = 0;
}

void GameplayScreen::Update(Game* game)
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

	HandleCollisions();
}

void GameplayScreen::Render(Game* game)
{
	// Clear color
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(game->renderer);

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
	for (std::vector<Wall*>::iterator iter = levelManager.wallList.begin();
		iter != levelManager.wallList.end(); ++iter)
	{
		(*iter)->Render(game->renderer);
	}

	// Render each remaining pellet on the board
	for (std::vector<Pellet*>::iterator iter = levelManager.pelletList.begin();
		iter != levelManager.pelletList.end(); ++iter)
	{
		(*iter)->Render(game->renderer);
	}

	// Render the player
	player->Render(game->renderer);

	// Render the AI
	for (std::vector<Ghost*>::iterator iter = ghostList.begin(); iter != ghostList.end(); ++iter)
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
	for (std::vector<Node*>::iterator iter = levelManager.legalPlayingNodes.begin();
		iter != levelManager.legalPlayingNodes.end(); ++iter)
	{
		// Check if player is inside a node
		if (Utils::CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
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
	for (std::vector<Wall*>::iterator iter = levelManager.wallList.begin(); iter != levelManager.wallList.end(); ++iter)
	{
		// Check if the player collides with a wall
		if (Utils::CollisionChecker(player->GetSpriteRect(), (*iter)->GetBoundingRect()))
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
			if (Utils::CollisionChecker((*aiIter)->GetBoundingRect(), (*iter)->GetBoundingRect()))
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
		if (Utils::CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
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
		if (Utils::CollisionChecker(player->GetBoundingRect(), (*iter)->GetBoundingRect()))
		{

			if (player->Kill()) isLevelOver = true;
		}
	}

}