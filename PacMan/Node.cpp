#include "Node.h"

Node::Node(float x, float y)
{
	location.x = x;
	location.y = y;
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