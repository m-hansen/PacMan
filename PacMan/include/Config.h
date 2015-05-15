#pragma once
#include <SDL.h>
#include <string>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"

namespace Config
{
	enum BoardSizeEnum
	{
		Small = 1,
		Medium = 2,
		Large = 3
	};

	// External config vars
	const extern std::string debugLogFolder;
	const extern std::string highScoresFile;
	const extern std::string key;
	extern int gridSize;
	extern int numberOfHorizontalTiles;
	extern int numberOfVerticalTiles;
	extern int screenWidth;
	extern int screenHeight;
	extern int highScoreTableCapacity;
	extern float sfxVol;
	extern float musicVol;
	extern bool isFullScreen;
	extern bool isSoundMuted;
	extern float joystickDeadZone;
	extern SDL_Color boardColor;

	// Internal constants
	const int GRID_SIZE_MULTIPLIER = 8;

	// Functions
	bool LoadConfig(std::string& filename);
	bool SaveConfig(std::string& filename);
}