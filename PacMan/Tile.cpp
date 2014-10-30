#include "Tile.h"

const static int TILE_SIZE = 8;

Tile::Tile(int x, int y, SDL_Texture* texture)
{
	position.x = x;
	position.y = y;
	tileTexture = texture;
	Initialize();
}

Tile::Tile(Vector2 pos, SDL_Texture* texture)
{
	position = pos;
	tileTexture = texture;
	Initialize();
}

Tile::~Tile()
{
}

void Tile::Initialize()
{
	int offset = (int)(TILE_SIZE / 2);
	boundingRect.x = position.x - offset;
	boundingRect.y = position.y - offset;
	boundingRect.w = TILE_SIZE;
	boundingRect.h = TILE_SIZE;
}

void Tile::Render(SDL_Renderer* renderer)
{
	SDL_Surface* sur = IMG_Load("tile.png");
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_RenderCopy(renderer, text, NULL, &boundingRect);
	//SDL_RenderDrawRect(renderer, boundingRect);
}

