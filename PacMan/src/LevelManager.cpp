#include "LevelManager.h"

LevelManager LevelManager::levelManager;

LevelManager::LevelManager()
{
}

LevelManager::~LevelManager()
{
}

void LevelManager::InitializeLevel()
{
	fprintf(stdout, "Loading new level\n");

	// Create the graph, load the level data, and calculate the edges
	level = new Graph();
	LoadLevelData(*currentLevel);
	FindEdges();

	// Initialize the pathfinder with the level data
	pathfinder = new Pathfinder(level);

	// Initialize the player
	player = new Player();
	player->ResetPosition();

	// Load the AI
	// TODO update scatter node locations
	ghostList.push_back(
		new Ghost("redEnemy", 12.0f, 5.0f, pathfinder, legalPlayingNodes[0], DirectionEnum::Left)
		);

	ghostList.push_back(
		new Ghost("pinkEnemy", 15.0f, 5.0f, pathfinder, legalPlayingNodes[0], DirectionEnum::Right)
		);

	ghostList.push_back(
		new Ghost("blueEnemy", 9.0f, 5.0f, pathfinder, legalPlayingNodes[0], DirectionEnum::Down)
		);

	ghostList.push_back(
		new Ghost("orangeEnemy", 18.0f, 5.0f, pathfinder, legalPlayingNodes[0], DirectionEnum::Down)
		);

	// Start the timer
	startLevelTimer.Start();
}

void LevelManager::CleanupLevel()
{
	fprintf(stdout, "Unloading level\n");

	startLevelTimer.Stop();

	// Free the player
	delete (player);
	player = NULL;

	// Free all of the AI
	for (std::vector<Ghost*>::iterator iter = ghostList.begin();
		iter != ghostList.end(); ++iter)
	{
		delete (*iter);
		(*iter) = NULL;
	}

	// erase the old lists
	legalPlayingNodes.clear();
	pelletList.clear();
	wallList.clear();
	ghostList.clear();

	// Delete the pathfinder
	delete (pathfinder);
	pathfinder = NULL;

	// Delete the level
	delete (level);
	level = NULL;

	// Reset the static id variable for the node class
	Node::ResetNodeCounter();
}

void LevelManager::LoadLevelData(std::string levelData)
{
	fprintf(stdout, "Loading level: %s\n", levelData.c_str());

	const int ASCII_NUM_OFFSET = 48;
	const int WALL_OFFSET = Config::gridSize / 2;

	std::ifstream inputStream;
	inputStream.open(levelData);
	std::string line = "";
	int lineNumber = 0;

	// Read each line in the level data file
	while (getline(inputStream, line))
	{
		// Get each tile in a line
		for (int i = 0; i < line.length(); i++)
		{
			// string::at returns a char so we will subtract the ascii offset value
			// and store it as an integer
			int nodeType = line.at(i) - ASCII_NUM_OFFSET;

			Node* node = NULL;

			switch (nodeType)
			{
			case NodeTypeEnum::EmptyNode:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::EmptyNode);
				level->AddNode(node);
				legalPlayingNodes.push_back(node);
				break;
			case NodeTypeEnum::PelletNode:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::PelletNode);
				level->AddNode(node);
				pelletList.push_back(new Pellet(node));
				legalPlayingNodes.push_back(node);
				break;
			case NodeTypeEnum::PowerPelletNode:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::PowerPelletNode);
				level->AddNode(node);
				pelletList.push_back(new Pellet(node));
				legalPlayingNodes.push_back(node);
				break;
			case NodeTypeEnum::WallNode:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::WallNode);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			case NodeTypeEnum::TopLeftCorner:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::TopLeftCorner);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall_top_left"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			case NodeTypeEnum::TopRightCorner:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::TopRightCorner);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall_top_right"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			case NodeTypeEnum::BottomLeftCorner:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::BottomLeftCorner);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall_bottom_left"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			case NodeTypeEnum::BottomRightCorner:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::BottomRightCorner);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall_bottom_right"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			case NodeTypeEnum::HorizontalWall:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::HorizontalWall);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall_horizontal"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			case NodeTypeEnum::VerticalWall:
				// Create on the heap
				node = new Node(i * Config::gridSize, lineNumber * Config::gridSize, NodeTypeEnum::VerticalWall);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall_vertical"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET + (node->GetPosition().y), Config::gridSize, Config::gridSize));
				break;
			}

			//printf("%c ", line.at(i));
		}

		//printf("\n");
		lineNumber++;
	}
	inputStream.close();
}

void LevelManager::ResetAgentPositions()
{
	// Reset the player's position
	player->ResetPosition();

	// Reset the AI
	Ghost::ChangeState(Scatter);
	for (auto iter = ghostList.begin(); iter != ghostList.end(); ++iter)
	{
		(*iter)->Respawn();
	}
}

void LevelManager::Update(Uint32 deltaTime)
{
	// Break out of the update function if the start level timer is still counting down
	// TODO enable start timer
	/*if ((startLevelTimer.IsStarted()) && (startLevelTimer.GetTicks() < 3000))
	{
		printf("Starting in ");
		if (startLevelTimer.GetTicks() > 2000) printf("1\n");
		else if (startLevelTimer.GetTicks() > 1000) printf("2\n");
		else if (startLevelTimer.GetTicks() > 0) printf("3\n");
		return;
	}*/

	if (startLevelTimer.IsStarted()) startLevelTimer.Stop();

	// Update the player
	player->Update(deltaTime);

	// Update the AI
	for (int i = 0; i < ghostList.size(); i++)
		ghostList[i]->Update(deltaTime);

	// Update the power pellet animations
	for (int i = 0; i < pelletList.size(); i++)
	{
		if (pelletList[i]->GetType() == PowerPelletNode)
			pelletList[i]->PlayBlinkAnimation(500); // parameter is the blink rate in ms
	}
}

void LevelManager::FindEdges()
{
	fprintf(stdout, "Calculating graph edges\n");

	FILE* ofp = fopen((Config::debugLogFolder + "GraphEdges.txt").c_str(), "a");

	// Print the level header
	std::string heavyLineBreak = "";
	for (int i = 0; i < (*currentLevel).length(); ++i)
		heavyLineBreak += '=';
	fprintf(ofp, "%s\n%s\n%s\n", heavyLineBreak.c_str(), (*currentLevel).c_str(), heavyLineBreak.c_str());

	for (std::vector<Node*>::iterator iter = legalPlayingNodes.begin(); iter != legalPlayingNodes.end(); ++iter)
	{
		// Write the graph node to file
		fprintf(ofp, "%d -> ", (*iter)->GetNodeId());

		for (std::vector<Node*>::iterator iter2 = legalPlayingNodes.begin(); iter2 != legalPlayingNodes.end(); ++iter2)
		{
			if (iter != iter2)
			{
				// Get the distance between nodes
				float deltaX = (*iter)->GetPosition().x - (*iter2)->GetPosition().x;
				float deltaY = (*iter)->GetPosition().y - (*iter2)->GetPosition().y;

				// Get the absolute value if needed
				if (deltaX < 0) deltaX *= -1;
				if (deltaY < 0) deltaY *= -1;

				if ((deltaX == Config::gridSize && deltaY == 0) || 
					(deltaX == 0 && deltaY == Config::gridSize))
				{
					// We are within the range to be considered an edge
					(*iter)->AddNeighborNode((*iter2));

					// Write the graph edges (neighbor nodes) to file
					fprintf(ofp, "%d ", (*iter2)->GetNodeId());
				}
			}
		}

		fprintf(ofp, "\n");
	}

	fprintf(ofp, "\n\n");
	fclose(ofp);
}

// Construct a list of all levels and their location on disk
void LevelManager::CreateLevelList(std::string dataLoc, std::vector<std::string> levelList)
{
	for (std::vector<std::string>::iterator iter = levelList.begin();
		iter != levelList.end(); ++iter)
	{
		levels.push_back(dataLoc + "/" + (*iter));
	}

	// Set the current level
	currentLevel = levels.begin();
}

bool LevelManager::NextLevel()
{
	// We are already at the last level - return
	if (currentLevel + 1 >= levels.end())
		return false;

	// Load in the next level
	CleanupLevel();
	++currentLevel;
	InitializeLevel();
	return true;

}

bool LevelManager::PreviousLevel()
{
	// We are already at the first level - return
	if (currentLevel <= levels.begin())
		return false;

	// Load in the previous level
	CleanupLevel();
	--currentLevel;
	InitializeLevel();
	return true;
}

// Returns true if the animation is complete - false otherwise
bool LevelManager::LevelCompletedAnimation()
{
	// The level is over
	if (!endLevelTimer.IsStarted())
		endLevelTimer.Start();

	const int END_LEVEL_PAUSE_LEN = 2500;
	const int NUM_FLASHES = 7;
	float  flashRate = END_LEVEL_PAUSE_LEN / NUM_FLASHES;

	int r = Config::boardColor.r;
	int g = Config::boardColor.g;
	int b = Config::boardColor.b;
	if (endLevelTimer.GetTicks() > flashRate * 6)
	{
		r = Config::boardColor.r, g = Config::boardColor.g, b = Config::boardColor.b;
	}
	else if (endLevelTimer.GetTicks() > flashRate * 5)
	{
		r = 255; g = 255; b = 255;
	}
	else if (endLevelTimer.GetTicks() > flashRate * 4)
	{
		r = Config::boardColor.r, g = Config::boardColor.g, b = Config::boardColor.b;
	}
	else if (endLevelTimer.GetTicks() > flashRate * 3)
	{
		r = 255; g = 255; b = 255;
	}
	else if (endLevelTimer.GetTicks() > flashRate * 2)
	{
		r = Config::boardColor.r, g = Config::boardColor.g, b = Config::boardColor.b;
	}
	else if (endLevelTimer.GetTicks() > flashRate)
	{
		r = 255; g = 255; b = 255;
	}

	SDL_SetTextureColorMod(TextureManager::GetTexture("wall"), r, g, b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_top_left"), r, g, b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_top_right"), r, g, b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_bottom_left"), r, g, b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_bottom_right"), r, g, b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_horizontal"), r, g, b);
	SDL_SetTextureColorMod(TextureManager::GetTexture("wall_vertical"), r, g, b);

	if (endLevelTimer.GetTicks() > END_LEVEL_PAUSE_LEN)
	{
		endLevelTimer.Stop();
		SDL_SetTextureColorMod(TextureManager::GetTexture("wall"), 
			Config::boardColor.r, Config::boardColor.g, Config::boardColor.b);
		return true;
	}

	return false;
}