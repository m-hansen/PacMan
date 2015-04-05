#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"
#include "Config.h"

const int MAX_TABLE_ENTRIES = 10;

struct TableEntry
{
	std::string name;
	int score;
};

class HighScoreTable
{
public:
	HighScoreTable();
	~HighScoreTable();
	void LoadHighScores(const std::string filename);
	void SaveHighScores(const std::string filename);
	bool UploadScore(std::string name, int score);
	void PrintHighScores();
private:
	void Cleanup();
	void SortAscending();
	void Swap(TableEntry* a, TableEntry* b);
	TableEntry* highScores[MAX_TABLE_ENTRIES];
};