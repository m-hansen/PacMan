#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"

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
private:
	void Cleanup();
	void LoadHighScores(const std::string filename);
	void SaveHighScores(const std::string filename);
	int* GetHighScores();
	bool UploadScore(int score);
	void SortAscending();
	void Swap(TableEntry* a, TableEntry* b);
	TableEntry* highScores[MAX_TABLE_ENTRIES];
};