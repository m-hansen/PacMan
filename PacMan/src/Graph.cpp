#include "Graph.h"

Graph::Graph()
{

}

Graph::~Graph()
{
	// Free all nodes in the graph
	for (std::vector<Node*>::iterator iter = allNodes.begin();
		iter != allNodes.end(); ++iter)
	{
		delete (*iter);
		(*iter) = NULL;
	}
}

// Assume this graph will be a rectangle
void Graph::GenerateGraph(int width, int height)
{
	fprintf(stdout, "[GenerateGraph] depreciated - this only creates a graph of empty nodes\n");

	int tempId = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			allNodes.push_back(new Node(j, i, NodeTypeEnum::EmptyNode));
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