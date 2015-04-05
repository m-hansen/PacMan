#include "HighScoreTable.h"

HighScoreTable::HighScoreTable()
{
	// Initialize the high scores
	for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		highScores[i] = NULL;
	}
}

HighScoreTable::~HighScoreTable()
{
	Cleanup();
}

void HighScoreTable::Cleanup()
{
	// Free the table memory
	for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		if (highScores[i] != NULL)
		{
			delete (highScores[i]);
			highScores[i] = NULL;
		}
	}
}

void HighScoreTable::LoadHighScores(const std::string filename)
{
	// Make sure the table is clean before loading data
	Cleanup();

	std::ifstream inputFile;
	inputFile.open(filename.c_str(), std::ifstream::in);

	if (!inputFile.is_open())
	{
		printf("Error: Could not open file: %s\n", filename.c_str());
		return;
	}

	int index = 0;
	std::string line;

	// Get each line up to the maximum number of table entries
	// any data that exceeds the number of max entries will be discarded
	// regardless of whether the score is higher
	while (std::getline(inputFile, line)
		&& index < MAX_TABLE_ENTRIES)
	{
		const char DELIM = ' ';
		bool foundDelim = false;
		std::string nameStr;
		std::string scoreStr;

		Utils::Decrypt(line, Config::key);

		// Split the string
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == DELIM)
			{
				foundDelim = true;
				continue;
			}

			if (!foundDelim)
				nameStr += line[i];
			else
				scoreStr += line[i];
		}

		// Add existing score to the table
		highScores[index] = new TableEntry{ nameStr, atoi(scoreStr.c_str()) };

		index++;
	}

	inputFile.close();
}

void HighScoreTable::SaveHighScores(const std::string filename)
{
	std::ofstream outputFile;
	outputFile.open(filename);

	for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		if (highScores[i] != NULL)
		{
			std::string line;
			line += highScores[i]->name + " " + 
				std::to_string(highScores[i]->score);

			Utils::Encrypt(line, Config::key);

			outputFile << line << std::endl;
		}
	}

	outputFile.close();
}

int* HighScoreTable::GetHighScores()
{
	int temp[MAX_TABLE_ENTRIES];
	for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		if (highScores[i] != NULL)
		{
			temp[i] = highScores[i]->score;
		}
	}
	return temp;
}

// Return true if the score was accepted as a high score
bool HighScoreTable::UploadScore(std::string name, int score)
{
	for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		if (highScores[i] == NULL)
		{
			highScores[i] = new TableEntry{ name, score };
			return true;
			// TODO - insertion sort?
		}
	}

	return false;
}

void HighScoreTable::SortAscending()
{
	for (int i = 1; i < MAX_TABLE_ENTRIES; i++)
	{
		if (highScores[i - 1] != NULL && highScores[i] != NULL)
		{
			if (highScores[i]->score > highScores[i - 1]->score)
			{
				/*TableEntry temp = *highScores[i];
				highScores[i] = highScores[i - 1];
				highScores[i - 1] = &temp;*/

				Swap(highScores[i], highScores[i - 1]);
			}
		}
	}
}

void HighScoreTable::Swap(TableEntry* a, TableEntry* b)
{
	// TODO this is wrong -- they are pointers!!
	TableEntry* temp = a;
	a = b;
	b = temp;
}

void HighScoreTable::PrintHighScores()
{
	printf("High Scores:\nName\tScore\n---------------\n");
	for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		if (highScores[i] != NULL)
		{
			printf("%s\t%d\n", highScores[i]->name.c_str(), highScores[i]->score);
		}
	}
	printf("---------------\n");
}