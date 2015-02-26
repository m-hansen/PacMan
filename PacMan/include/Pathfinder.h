#pragma once
#include <vector>
#include "Node.h"

class Pathfinder
{
public:
	Pathfinder(std::vector<Node*> nodes) : world(nodes) {}
	~Pathfinder() {}
	std::vector<Node*> CalculateAStar(Node* startingNode, Node* targetNode);
private:
	void ConstructPath(Node* node);
	void RecursivelyConstructPath(Node* target);
	void PrintShortestPath();
	void MoveFromOpenToClosed(std::vector<Node*>::iterator iter);
	std::vector<Node*> world;
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	std::vector<Node*> shortestPath;
};