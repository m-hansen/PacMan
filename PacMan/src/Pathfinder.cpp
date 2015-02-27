#include "Pathfinder.h"

std::vector<Node*> Pathfinder::CalculateAStar(Node* startingNode, Node* targetNode)
{
	// Add the starting node to the open list
	openList.push_back(startingNode);
	startingNode->CalculateHeuristic(targetNode);
	startingNode->CalculateTotalCost();

	// Loop while the open list contains nodes
	while (!openList.empty())
	{
		Node* currentNode = NULL;
		std::vector<Node*>::iterator currentNodeIter;

		// Search the open list for the lowest F cost
		for (auto iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (currentNode == NULL || (*iter)->GetTotalCost() < currentNode->GetTotalCost())
			{
				currentNode = (*iter);
				currentNodeIter = iter;
			}
		}

		// Stop searching when we have added our target node to the closed list
		if (currentNode == NULL || currentNode == targetNode)
			break;

		// Remove the checked node from the open list and add to the closed list
		openList.erase(currentNodeIter);
		closedList.push_back(currentNode);

		std::vector<Node*> neighbors = currentNode->GetNeighborNodes();
		// Loop over each neighboring node
		for (int k = 0; k < neighbors.size(); k++)
		{
			bool onClosedList = false;
			bool onOpenList = false;

			// Break if the node is not legal
			if (!neighbors[k]->IsLegal())
				continue;
				
			// Check to see if the node is already on the closed list
			for (int l = 0; l < closedList.size(); l++)
			{
				if (neighbors[k] == closedList[l])
				{
					onClosedList = true;
					break;
				}
			}

			// Break if on closed list
			if (onClosedList)
				continue;

			// Check if node is on the open list
			for (int l = 0; l < openList.size(); l++)
			{
				if (neighbors[k] == openList[l])
				{
					onOpenList = true;
					break;
				}
			}
				
			if (!onOpenList)
			{
				// Add node to the open list
				openList.push_back(neighbors[k]);

				// Set the parent node to the current node
				neighbors[k]->SetParentNode(currentNode);

				// Calculate the costs
				neighbors[k]->CalculateMovementCost(currentNode);
				neighbors[k]->CalculateHeuristic(targetNode);
				neighbors[k]->CalculateTotalCost();
			}
			else
			{
				// node is already on the open list
				if (neighbors[k]->GetMovementCost() < currentNode->GetMovementCost())
				{
					// Set a new parent node
					neighbors[k]->SetParentNode(currentNode);

					// Recalculate G and F costs
					neighbors[k]->CalculateMovementCost(currentNode);
					neighbors[k]->CalculateHeuristic(targetNode);
					neighbors[k]->CalculateTotalCost();
				}
			}
		} // end for each neighbor
	} // end while open list is not empty

	RecursivelyConstructPath(targetNode);
	PrintShortestPath();
	return shortestPath;
}

void Pathfinder::PrintShortestPath()
{
	FILE* ofp = fopen((DEBUG_LOG_FOLDER + "ShortestPath.txt").c_str(), "a");
	for (int i = 0; i < shortestPath.size(); i++)
	{
		fprintf(ofp, "%d ", shortestPath[i]->GetNodeId());
	}
	fclose(ofp);
}

void Pathfinder::RecursivelyConstructPath(Node* target)
{
	if (target->GetParentNode() != NULL)
		RecursivelyConstructPath(target->GetParentNode());

	shortestPath.push_back(target);
}
