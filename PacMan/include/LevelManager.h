#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Global.h"
#include "TextureManager.h"
#include "NodeTypeEnum.h"
#include "Consumable.h"
#include "Graph.h"
#include "Pellet.h"
#include "Ghost.h"
#include "Player.h"
#include "Wall.h"

class LevelManager
{
public:
	~LevelManager();
	static LevelManager* Instance()	{ return &levelManager; }
	void InitializeLevel();
	void CleanupLevel();
	void LoadLevelData(std::string levelData);
	void FindEdges();
	void CreateLevelList(std::string dataLoc, 
		std::vector<std::string> levelList);
	void ResetAI();
	bool NextLevel();
	bool PreviousLevel();
	Graph* GetLevel() { return level; }
	Player* GetPlayer() { return player; }
	std::vector<Node*>& GetLegalNodes() { return legalPlayingNodes; }
	std::vector<Pellet*>& GetPellets() { return pelletList; }
	std::vector<Wall*>& GetWalls() { return wallList; }
	std::vector<Ghost*>& GetGhosts() { return ghostList; }
private:
	static LevelManager levelManager;
	Player* player;
	Graph* level;
	std::vector<Node*> legalPlayingNodes;
	std::vector<Pellet*> pelletList;
	std::vector<Wall*> wallList;
	std::vector<Ghost*> ghostList;
	std::vector<std::string> levels;
	std::vector<std::string>::iterator currentLevel;
protected:
	LevelManager();
};