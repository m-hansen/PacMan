#pragma once
#include <vector>
#include "Graph.h"
#include "Node.h"

class Pathfinder
{
public:
	Pathfinder(Graph* g) : world(g) {}
	~Pathfinder() {}
	std::vector<Node*> CalculateAStar(Node* startingNode, Node* targetNode);
private:
	int CalculateHeuristic(Node* start, Node* target);
	void PrintShortestPath();
	void InitializeAStar();
	void RecursivelyConstructPath(Node* target);
	Graph* world;
	//std::vector<Node*> openList;
	//std::vector<Node*> closedList;
	std::vector<Node*> shortestPath;
};