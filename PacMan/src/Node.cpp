#include "Node.h"

int Node::id = 0;

Node::Node(float x, float y, NodeTypeEnum nodeType)
{
	currentId = id++;

	boundingRect.w = Config::gridSize;
	boundingRect.h = Config::gridSize;
	boundingRect.x = x;
	boundingRect.y = y;

	parentNode = NULL;
	type = nodeType;

	if ((nodeType == NodeTypeEnum::EmptyNode) ||
		(nodeType == NodeTypeEnum::PelletNode) ||
		(nodeType == NodeTypeEnum::PowerPelletNode))
	{
		isLegalPlayingNode = true;
	}
	else
	{
		isLegalPlayingNode = false;
	}

	InitializePathfindingInfo();
}

void Node::InitializePathfindingInfo()
{
	// Initialize G, H, and F
	movementCost = 0;
	heuristic = 0;
	totalCost = 0;
	parentNode = NULL;
}

void Node::Render(SDL_Renderer* renderer)
{
	SDL_RenderDrawRect(renderer, &boundingRect);
}

void Node::AddNeighborNode(Node* neighbor)
{
	neighborNodes.push_back(neighbor);
}

std::vector<Node*> Node::GetNeighborNodes()
{
	return neighborNodes;
}

Vector2f Node::GetPosition()
{
	return Vector2f{ boundingRect.x, boundingRect.y };
}

NodeTypeEnum Node::GetType()
{
	return type;
}

SDL_Rect* Node::GetBoundingRect()
{
	return &boundingRect;
}

int Node::GetNodeId()
{
	return currentId;
}

// Returns true if the neighbor node id was found, false otherwise
bool Node::SearchNeighborId(int id)
{
	if (neighborNodes.empty())
		return false;

	for (int i = 0; i < neighborNodes.size(); i++)
	{
		if (neighborNodes.at(i)->id == id)
			return true;
	}
	return false;
}

void Node::CalculateMovementCost(Node* node)
{
	// For now, we will assume the movement cost is always 10
	// This will change when we implement diagonal pathfinding
	movementCost = 10 + node->GetMovementCost();
}

void Node::CalculateHeuristic(Node* target)
{
	int horizontalSpaces = std::abs ((currentId % Config::numberOfHorizontalTiles) - (target->GetNodeId() % Config::numberOfHorizontalTiles));
	int verticalSpaces = std::abs((currentId / Config::numberOfHorizontalTiles) - (target->GetNodeId() / Config::numberOfHorizontalTiles));
	heuristic = (horizontalSpaces + verticalSpaces) * 10;
}

void Node::CalculateTotalCost()
{
	totalCost = movementCost + heuristic;
}