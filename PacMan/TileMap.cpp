#include "TileMap.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

void TileMap::GenerateMap()
{
	const int TILE_SIZE = 8;

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// Allocate on the heap so we can access throughout the life of our program
			tileMap[j][i] = new Tile(j * TILE_SIZE, i * TILE_SIZE);

			// Write out the position of each tile as it's being created
			printf("New tile position: (%d,%d)\n", tileMap[j][i]->GetPosition().x, 
				tileMap[j][i]->GetPosition().y);
		}
	}
}

void TileMap::Render(SDL_Renderer* renderer)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			// FIX-ME 0xCCCCCC lost ref to texture :(
			tileMap[j][i]->Render(renderer);
		}
	}
}

Tile** TileMap::GetTileMap()
{
	printf("Hey, you! Implement the Tile** TileMap::GetTileMap() function before calling it!\n");
	return NULL;
}