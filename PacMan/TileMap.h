#pragma once
#include <stdio.h>
#include "Tile.h"

const int MAP_WIDTH = 28;
const int MAP_HEIGHT = 36;

class TileMap
{
public:
	TileMap();
	~TileMap();
	void GenerateMap();
	void Render(SDL_Renderer* renderer);
	Tile** GetTileMap();
private:
	float mapWidth;
	float mapHeight;
	Tile* tileMap[MAP_WIDTH][MAP_HEIGHT];
};