#pragma once

#include <SDL.h>
#include <vector>
#include "Edge.h"
#include "Utils.h"
#include "NodeType.h"

class Node
{
public:
	Node(float x, float y, NodeType nodetype);
	~Node();
	void Node::Render(SDL_Renderer* renderer);
	void AddEdge(Edge* edge);
	std::vector<Edge*> GetEdges();
	Vector2f GetLocation();
	NodeType Node::GetType();
	SDL_Rect* GetBoundingRect();
	int GetNodeId();
private:
	Vector2f location;
	std::vector<Edge*> edgeList;
	NodeType type;
	SDL_Rect boundingRect;
	static int id;
	int currentId;
};
