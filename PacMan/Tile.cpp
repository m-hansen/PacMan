#include "Tile.h"

const static int TILE_SIZE = 8;

Tile::Tile(int x, int y)
{
	position.x = x;
	position.y = y;
	
	Initialize();
}

Tile::Tile(Vector2 pos)
{
	position = pos;
	Initialize();
}

Tile::~Tile()
{
}

void Tile::Initialize()
{
	// Get the texture
	tileTexture = TextureManager::GetTexture("tile");

	// Set the bounds for a tile
	int offset = (int)(TILE_SIZE / 2);
	boundingRect.x = position.x - offset;
	boundingRect.y = position.y - offset;
	boundingRect.w = TILE_SIZE;
	boundingRect.h = TILE_SIZE;
}

void Tile::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, tileTexture, NULL, &boundingRect);
}

Vector2 Tile::GetPosition()
{
	return position;
}

