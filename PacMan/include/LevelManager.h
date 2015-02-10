#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Global.h"
#include "TextureManager.h"
#include "NodeType.h"
#include "Consumable.h"
#include "ConsumableType.h"
#include "Graph.h"
#include "Pellet.h"
#include "Wall.h"

class LevelManager
{
public:
	void LoadLevel(std::string levelData);
	void FindEdges();
	std::vector<Node*> legalPlayingNodes;
	std::vector<Pellet*> pelletList;
	std::vector<Wall*> wallList;
private:
	Graph level;
};