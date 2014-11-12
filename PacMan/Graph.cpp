#include "Graph.h"

Graph::Graph()
{

}

Graph::~Graph()
{

}

// Assume this graph will be a rectangle
void Graph::GenerateGraph(int width, int height)
{
	int tempId = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			allNodes.push_back(new Node(j, i));
			printf("%d ", tempId);
			tempId++;
		}
		printf("\n");
	}
}

std::vector<Node*>* Graph::GetAllNodes()
{
	return &allNodes;
}

void Graph::AddNode(Node* node)
{
	allNodes.push_back(node);
}