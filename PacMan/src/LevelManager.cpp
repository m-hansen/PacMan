#include "LevelManager.h"

void LevelManager::LoadLevel(std::string levelData)
{
	const int ASCII_NUM_OFFSET = 48;

	std::ifstream inputStream;
	inputStream.open(levelData);
	std::string line = "";
	int lineNumber = 0;

	// Read each line in the level data file
	while (getline(inputStream, line))
	{
		// Get each tile in a line
		for (int i = 0; i < line.length(); i++)
		{

			// string::at returns a char so we will subtract the ascii offset value
			// and store it as an integer
			int nodeType = line.at(i) - ASCII_NUM_OFFSET;

			Node* node = NULL;

			switch (nodeType)
			{
			case NodeType::EmptyNode:
				//node->SetContents(TileTypeEnum::Empty);
				//node->SetTexture(TextureManager::GetTexture("tile"));

				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::EmptyNode);
				level.AddNode(node);
				legalPlayingNodes.push_back(node);
				break;
			case NodeType::PelletNode:
				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::PelletNode);
				level.AddNode(node);

				pelletList.push_back(new Pellet(node));

				//tile->SetContents(TileTypeEnum::Pellet);
				legalPlayingNodes.push_back(node);
				//tile->SetTexture(TextureManager::GetTexture("pellet"));
				break;
			case NodeType::PowerPelletNode:
				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::PowerPelletNode);
				level.AddNode(node);
				//tile->SetContents(TileTypeEnum::PowerPellet);
				legalPlayingNodes.push_back(node);
				//tile->SetTexture(TextureManager::GetTexture("power pellet"));
				break;
			case NodeType::WallNode:
				// Create on the heap
				node = new Node(i * 1, lineNumber * 1, NodeType::WallNode);
				level.AddNode(node);
				//tile->SetContents(TileTypeEnum::Wall);
				//tile->SetTexture(TextureManager::GetTexture("wall"));
				wallList.push_back(new Wall(node));
				break;
			}

			// TODO mem will eventually need to be freed=
			//node = NULL;
			printf("%c ", line.at(i));
		}
		printf("\n");
		lineNumber++;
	}
}

void LevelManager::FindEdges()
{
	std::ofstream outputFile("graph_edges.txt");

	for (std::vector<Node*>::iterator iter = legalPlayingNodes.begin(); iter != legalPlayingNodes.end(); ++iter)
	{
		// Write the graph node to file
		outputFile << (*iter)->GetNodeId() << " -> ";

		for (std::vector<Node*>::iterator iter2 = legalPlayingNodes.begin(); iter2 != legalPlayingNodes.end(); ++iter2)
		{
			if (iter != iter2)
			{
				// Get the distance between nodes
				float deltaX = (*iter)->GetLocation().x - (*iter2)->GetLocation().x;
				float deltaY = (*iter)->GetLocation().y - (*iter2)->GetLocation().y;

				// Get the absolute value if needed
				if (deltaX < 0) deltaX *= -1;
				if (deltaY < 0) deltaY *= -1;

				// Adjust based on scale
				/*deltaX /= G_SIZE;
				deltaY /= G_SIZE;*/

				//if ((deltaX <= (1)) && (deltaY <= (1)))
				if ((deltaX == 1 && deltaY == 0) || 
					(deltaX == 0 && deltaY == 1))
				{
					// We are within the range to be considered an edge
					(*iter)->AddNeighborNode((*iter2));

					// Write the graph edges (neighbor nodes) to file
					outputFile << (*iter2)->GetNodeId() << " ";
				}
			}
		}

		outputFile << "\n";
	}
}