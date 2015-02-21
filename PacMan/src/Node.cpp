#include "Node.h"

int Node::id = 0;

Node::Node(float x, float y, NodeType nodeType)
{
	currentId = id++;

	location.x = x;
	location.y = y;
	type = nodeType;

	if ((nodeType == NodeType::EmptyNode) || 
		(nodeType == NodeType::PelletNode) || 
		(nodeType == NodeType::PowerPelletNode))
	{
		isLegalPlayingNode = true;
	}
	else
	{
		isLegalPlayingNode = false;
	}

	boundingRect.w = G_SIZE;
	boundingRect.h = G_SIZE;
	boundingRect.x = location.x * G_SIZE;
	boundingRect.y = location.y* G_SIZE;
}

Node::~Node()
{
	//// Free all memory from neighboring nodes
	//for (std::vector<Node*>::iterator iter = neighborNodes.begin(); iter != neighborNodes.end(); ++iter)
	//{
	//	delete (*iter);
	//	(*iter) = NULL;
	//}
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

Vector2f Node::GetLocation()
{
	return location;
}

NodeType Node::GetType()
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