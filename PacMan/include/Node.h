#pragma once

#include <SDL.h>
#include <vector>
#include "Global.h"
#include "Utils.h"
#include "NodeTypeEnum.h"

enum NodeDirection
{
	North, South, East, West
};

class Node
{
public:
	Node(float x, float y, NodeTypeEnum nodetype);
	~Node();
	void Node::Render(SDL_Renderer* renderer);
	void AddNeighborNode(Node* neighbor);
	std::vector<Node*> GetNeighborNodes();
	Vector2f GetLocation();
	NodeTypeEnum Node::GetType();
	SDL_Rect* GetBoundingRect();
	bool SearchNeighborId(int id);
	int GetNodeId();
	static void ResetNodeCounter() { id = 0; }
private:
	Vector2f location;
	std::vector<Node*> neighborNodes;
	NodeTypeEnum type;
	SDL_Rect boundingRect;
	static int id;
	int currentId;
	bool isLegalPlayingNode;
};
