#pragma once

#include <vector>
#include "Edge.h"
#include "Utils.h"

class Node
{
public:
	Node(float x, float y);
	~Node();
	void AddEdge(Edge* edge);
	std::vector<Edge*> GetEdges();
	Vector2f GetLocation();
private:
	Vector2f location;
	std::vector<Edge*> edgeList;
};
