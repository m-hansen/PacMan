//#pragma once
//#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include "Tile.h"
//
//const int MAP_WIDTH = 28;
//const int MAP_HEIGHT = 36;
//
//class TileMap
//{
//public:
//	TileMap();
//	~TileMap();
//	void LoadLevel(std::string levelData);
//	void GenerateMap();
//	void DestroyMap();
//	void Render(SDL_Renderer* renderer);
//	std::vector<Tile*> GetWalls();
//private:
//	float mapWidth;
//	float mapHeight;
//	Tile* map[MAP_WIDTH][MAP_HEIGHT];
//	std::vector<Tile*> wallList;
//};