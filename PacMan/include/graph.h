#pragma once

#include <vector>
#include "Node.h"

class Graph
{
public:
	Graph();
	~Graph();
	void GenerateGraph(int width, int height);
	std::vector<Node*>& GetAllNodes();
	void AddNode(Node* node);
private:
	std::vector<Node*> allNodes;
};