#pragma once
#include <SDL.h>
#include "TextureManager.h"
//#include "Sprite.h"

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
	Tile(int x, int y);
	Tile(Vector2 pos);
	~Tile();
	void Tile::Initialize();
	void Render(SDL_Renderer* renderer);
	Vector2 GetPosition();
private:
	friend class TextureManager;
	int tileSize;
	Vector2 position;
	SDL_Rect boundingRect;
	SDL_Texture* tileTexture;
};