#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "TextureManager.h"
#include "NodeType.h"
#include "Consumable.h"
#include "ConsumableType.h"
#include "Graph.h"

class LevelManager
{
public:
	void LoadLevel(std::string levelData);
	std::vector<Node*> legalPlayingNodes;
	std::vector<Consumable*> consumableList;
private:
	Graph level;
};