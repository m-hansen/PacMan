#include "Node.h"

int Node::id = 0;

Node::Node(float x, float y, NodeType nodeType)
{
	currentId = id++;

	location.x = x;
	location.y = y;
	type = nodeType;

	const int SIZE = 8;
	boundingRect.w = SIZE;
	boundingRect.h = SIZE;
	boundingRect.x = location.x * SIZE;
	boundingRect.y = location.y* SIZE;
}

Node::~Node()
{
	// Free any memory that holds the edges for this node
	for (std::vector<Edge*>::iterator iter = edgeList.begin(); iter != edgeList.end(); ++iter)
	{
		delete (*iter);
		(*iter) = NULL;
	}
}

void Node::Render(SDL_Renderer* renderer)
{
	SDL_RenderDrawRect(renderer, &boundingRect);
}

void Node::AddEdge(Edge* edge)
{
	edgeList.push_back(edge);
}

std::vector<Edge*> Node::GetEdges()
{
	return edgeList;
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