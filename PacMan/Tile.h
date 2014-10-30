#pragma once
#include <SDL.h>
#include "TextureManager.h"
#include "Sprite.h"

enum TileTypeEnum
{
	Wall = 0,
	Pellet = 1,
	PowerPellet = 2,
	Empty = 3
};

struct Vector2
{
	int x;
	int y;
};

class Tile
{
public:
	Tile(int x, int y, SDL_Texture* texture);
	Tile(Vector2 pos, SDL_Texture* texture);
	~Tile();
	void Tile::Initialize();
	void Render(SDL_Renderer* renderer);
private:
	int tileSize;
	Vector2 position;
	SDL_Rect boundingRect;
	SDL_Texture* tileTexture;
};