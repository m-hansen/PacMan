#include "TileMap.h"

TileMap::TileMap(TextureManager* manager)
{
	textureManager = manager;
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
			Tile tile(j, i, textureManager->GetTexture("tile"));
			tileMap[j][i] = &tile;
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
	return NULL;//tileMap;
}