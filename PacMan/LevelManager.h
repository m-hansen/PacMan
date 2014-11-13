#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "TextureManager.h"
#include "TileTypeEnum.h"
#include "Graph.h"

class LevelManager
{
public:
	void LoadLevel(std::string levelData);
private:
	std::vector<Node*> legalPlayingNodes;
	Graph level;
};