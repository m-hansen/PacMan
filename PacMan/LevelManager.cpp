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
			// Create on the heap
			Node* node = new Node(i * TILE_SIZE, lineNumber * TILE_SIZE);
			level.AddNode(node);

			// string::at returns a char so we will subtract the ascii offset value
			// and store it as an integer
			int tileType = line.at(i) - ASCII_NUM_OFFSET;

			switch (tileType)
			{
			case TileTypeEnum::Empty:
				//node->SetContents(TileTypeEnum::Empty);
				//node->SetTexture(TextureManager::GetTexture("tile"));
				legalPlayingNodes.push_back(node);
				break;
			case TileTypeEnum::Pellet:
				//tile->SetContents(TileTypeEnum::Pellet);
				legalPlayingNodes.push_back(node);
				//tile->SetTexture(TextureManager::GetTexture("pellet"));
				break;
			case TileTypeEnum::PowerPellet:
				//tile->SetContents(TileTypeEnum::PowerPellet);
				legalPlayingNodes.push_back(node);
				//tile->SetTexture(TextureManager::GetTexture("power pellet"));
				break;
			case TileTypeEnum::Wall:
				//tile->SetContents(TileTypeEnum::Wall);
				//tile->SetTexture(TextureManager::GetTexture("wall"));
				//wallList.push_back(tile);
				break;
			}

			node = NULL;
			printf("%c ", line.at(i));
		}
		printf("\n");
		lineNumber++;
	}
}