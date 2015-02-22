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

	// Initialize the player
	player = new Player();
	player->ResetPosition();

	// Load the AI
	ghostList.push_back(
		new Ghost("blinky", 12.0f, 5.0f, DirectionEnum::Left)
		);

	ghostList.push_back(
		new Ghost("pinky", 15.0f, 5.0f, DirectionEnum::Right)
		);

	ghostList.push_back(
		new Ghost("inky", 9.0f, 5.0f, DirectionEnum::Down)
		);

	ghostList.push_back(
		new Ghost("clyde", 18.0f, 5.0f, DirectionEnum::Down)
		);
}

void LevelManager::CleanupLevel()
{
	fprintf(stdout, "Unloading level\n");

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
				node = new Node(i * GRID_SIZE, lineNumber * GRID_SIZE, NodeTypeEnum::EmptyNode);
				level->AddNode(node);
				legalPlayingNodes.push_back(node);
				break;
			case NodeTypeEnum::PelletNode:
				// Create on the heap
				node = new Node(i * GRID_SIZE, lineNumber * GRID_SIZE, NodeTypeEnum::PelletNode);
				level->AddNode(node);
				pelletList.push_back(new Pellet(node));
				legalPlayingNodes.push_back(node);
				break;
			case NodeTypeEnum::PowerPelletNode:
				// Create on the heap
				node = new Node(i * GRID_SIZE, lineNumber * GRID_SIZE, NodeTypeEnum::PowerPelletNode);
				level->AddNode(node);
				pelletList.push_back(new Pellet(node));
				legalPlayingNodes.push_back(node);
				break;
			case NodeTypeEnum::WallNode:
				// Create on the heap
				const int WALL_OFFSET = GRID_SIZE / 2;
				node = new Node(i * GRID_SIZE, lineNumber * GRID_SIZE, NodeTypeEnum::WallNode);
				level->AddNode(node);
				wallList.push_back(new Sprite(
					TextureManager::GetTexture("wall"), WALL_OFFSET + (node->GetPosition().x),
					WALL_OFFSET  +(node->GetPosition().y), GRID_SIZE, GRID_SIZE));
				break;
			}

			printf("%c ", line.at(i));
		}

		printf("\n");
		lineNumber++;
	}
	inputStream.close();
}

void LevelManager::ResetAI()
{
	for (auto iter = ghostList.begin(); iter != ghostList.end(); ++iter)
	{
		(*iter)->Respawn();
	}
}

void LevelManager::FindEdges()
{
	fprintf(stdout, "Calculating graph edges\n");

	FILE* ofp = fopen((DEBUG_LOG_FOLDER + "GraphEdges.txt").c_str(), "a");

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

				if ((deltaX == GRID_SIZE && deltaY == 0) || 
					(deltaX == 0 && deltaY == GRID_SIZE))
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