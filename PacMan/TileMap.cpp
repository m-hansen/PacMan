#include "TileMap.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
	DestroyMap();
}

void TileMap::LoadLevel(std::string levelData)
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
			tileMap[i][lineNumber] = new Tile(i * TILE_SIZE, lineNumber * TILE_SIZE);
			Tile* tile = tileMap[i][lineNumber];

			// string::at returns a char so we will subtract the ascii offset value
			// and store it as an integer
			int tileType = line.at(i) - ASCII_NUM_OFFSET;

			switch (tileType)
			{
				case TileTypeEnum::Empty:
					tile->SetContents(TileTypeEnum::Empty);
					tile->SetTexture(TextureManager::GetTexture("tile"));
					break;
				case TileTypeEnum::Pellet:
					tile->SetContents(TileTypeEnum::Pellet);
					//tile->SetTexture(TextureManager::GetTexture("pellet"));
					break;
				case TileTypeEnum::PowerPellet:
					tile->SetContents(TileTypeEnum::PowerPellet);
					//tile->SetTexture(TextureManager::GetTexture("power pellet"));
					break;
				case TileTypeEnum::Wall:
					tile->SetContents(TileTypeEnum::Wall);
					tile->SetTexture(TextureManager::GetTexture("wall"));
					break;
			}

			tile = NULL;
			printf("%c ", line.at(i));
		}
		printf("\n");
		lineNumber++;
	}
}

void TileMap::GenerateMap()
{
	const int TILE_SIZE = 8;

	// Used for centering tiles
	const int OFFSET = TILE_SIZE / 2;

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// Allocate on the heap so we can access throughout the life of our program
			tileMap[j][i] = new Tile(OFFSET + j * TILE_SIZE, OFFSET + i * TILE_SIZE);

			// Write out the position of each tile as it's being created
			printf("New tile position: (%d,%d)\n", tileMap[j][i]->GetPosition().x, 
				tileMap[j][i]->GetPosition().y);
		}
	}
}

void TileMap::DestroyMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			delete (tileMap[j][i]);
		}
	}
}

void TileMap::Render(SDL_Renderer* renderer)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			tileMap[j][i]->Render(renderer);
		}
	}
}

Tile** TileMap::GetTileMap()
{
	printf("Hey, you! Implement the Tile** TileMap::GetTileMap() function before calling it!\n");
	return NULL;
}