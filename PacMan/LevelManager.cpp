#include "LevelManager.h"

void LevelManager::LoadLevel(std::string levelData)
{
	const int ASCII_NUM_OFFSET = 48;
	const int TILE_SIZE = 8;

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
			Consumable* c = NULL;

			switch (nodeType)
			{
			case NodeType::Empty:
				//node->SetContents(TileTypeEnum::Empty);
				//node->SetTexture(TextureManager::GetTexture("tile"));

				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::Empty);
				level.AddNode(node);
				legalPlayingNodes.push_back(node);
				break;
			case NodeType::Pellet:
				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::Pellet);
				level.AddNode(node);
				c = new Consumable(ConsumableType::CPellet, lineNumber, i);
				//tile->SetContents(TileTypeEnum::Pellet);
				legalPlayingNodes.push_back(node);
				//tile->SetTexture(TextureManager::GetTexture("pellet"));
				break;
			case NodeType::PowerPellet:
				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::PowerPellet);
				level.AddNode(node);
				//tile->SetContents(TileTypeEnum::PowerPellet);
				legalPlayingNodes.push_back(node);
				//tile->SetTexture(TextureManager::GetTexture("power pellet"));
				break;
			case NodeType::Wall:
				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::Wall);
				level.AddNode(node);
				//tile->SetContents(TileTypeEnum::Wall);
				//tile->SetTexture(TextureManager::GetTexture("wall"));
				//wallList.push_back(tile);
				break;
			}

			// TODO mem will eventually need to be freed=
			//node = NULL;
			printf("%c ", line.at(i));
		}
		printf("\n");
		lineNumber++;
	}
}