#pragma once

#include <SDL.h>
#include <vector>
#include "Global.h"
#include "Edge.h"
#include "Utils.h"
#include "NodeType.h"

enum NodeDirection
{
	North, South, East, West
};

class Node
{
public:
	Node(float x, float y, NodeType nodetype);
	~Node();
	void Node::Render(SDL_Renderer* renderer);
	void AddNeighborNode(Node* neighbor);
	std::vector<Node*> GetNeighborNodes();
	Vector2f GetLocation();
	NodeType Node::GetType();
	SDL_Rect* GetBoundingRect();
	bool SearchNeighborId(int id);
	int GetNodeId();
private:
	Vector2f location;
	std::vector<Node*> neighborNodes;
	NodeType type;
	SDL_Rect boundingRect;
	static int id;
	int currentId;
	bool isLegalPlayingNode;
};
