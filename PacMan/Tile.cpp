#include "Tile.h"

const static int TILE_SIZE = 8;
static int nextId = -1;

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
	// Generate a unique ID
	nextId++;
	uniqueId = nextId;

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

TileTypeEnum* Tile::GetContents()
{
	return &contents;
}

void Tile::SetContents(TileTypeEnum type)
{
	contents = type;
}

void Tile::SetTexture(SDL_Texture* texture)
{
	tileTexture = texture;
}

Vector2 Tile::GetPosition()
{
	return position;
}

SDL_Rect* Tile::GetBoundingRect()
{
	return &boundingRect;
}

int Tile::GetId()
{
	return uniqueId;
}