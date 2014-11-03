#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Tile.h"

const int MAP_WIDTH = 28;
const int MAP_HEIGHT = 36;

class TileMap
{
public:
	TileMap();
	~TileMap();
	void LoadLevel(std::string levelData);
	void GenerateMap();
	void DestroyMap();
	void Render(SDL_Renderer* renderer);
	Tile** GetTileMap();
private:
	float mapWidth;
	float mapHeight;
	Tile* tileMap[MAP_WIDTH][MAP_HEIGHT];
};