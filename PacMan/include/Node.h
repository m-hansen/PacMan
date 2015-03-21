#pragma once

#include <SDL.h>
#include <vector>
#include "Config.h"
#include "Utils.h"
#include "Enumerations/NodeTypeEnum.h"

enum NodeDirection
{
	North, South, East, West
};

class Node
{
public:
	Node(float x, float y, NodeTypeEnum nodetype);
	~Node() {}
	void InitializePathfindingInfo();
	void Node::Render(SDL_Renderer* renderer);
	void AddNeighborNode(Node* neighbor);
	std::vector<Node*> GetNeighborNodes();
	Vector2f GetPosition();
	NodeTypeEnum Node::GetType();
	SDL_Rect* GetBoundingRect();
	bool SearchNeighborId(int id);
	int GetNodeId();
	int GetTotalCost() const { return totalCost; }
	int GetMovementCost() const { return movementCost; }
	int GetHeuristic() const { return heuristic; }
	static void ResetNodeCounter() { id = 0; }
	bool IsLegal() const{ return isLegalPlayingNode; }
	void CalculateMovementCost(Node* node);
	void CalculateHeuristic(Node* target);
	void CalculateTotalCost();
	Node* GetParentNode() { return parentNode; }
	void SetParentNode(Node* newParent) { parentNode = newParent; }
private:
	std::vector<Node*> neighborNodes;
	NodeTypeEnum type;
	SDL_Rect boundingRect;
	static int id;
	int currentId;
	bool isLegalPlayingNode;
	Node* parentNode;
	int movementCost = 10;	// G value
	int heuristic;		// H value
	int totalCost;		// F value
};
