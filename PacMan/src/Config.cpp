#include "Config.h"

namespace Config
{
	const std::string debugLogFolder = "Logs/";
	const std::string highScoresFile = "Resources/scores.dat";
	const std::string key = "0bDSt673&doy$ns@d!vicHJ#tYnx8s"; // nothing serious, just a layer to prevent casual modifications
	int gridSize = BoardSizeEnum::Medium * GRID_SIZE_MULTIPLIER; // use increments of 8
	int numberOfHorizontalTiles = 28;
	int numberOfVerticalTiles = 36;
	int screenWidth = numberOfHorizontalTiles * gridSize; // 224 width with 8x8 tiles
	int screenHeight = numberOfVerticalTiles * gridSize; // 288 height with 8x8 tiles
	int highScoreTableCapacity = 10;
	bool isFullScreen = false;
	bool isSoundMuted = false;
	float joystickDeadZone = 15000.0f;
}

bool Config::LoadConfig(std::string& filename)
{
	using boost::property_tree::ptree;

	// Create a property tree
	ptree pt;

	// Load the XML into the tree
	read_xml(filename, pt);

	// Read the settings from config file
	int boardSize = pt.get("config.size", 2);
	isFullScreen = pt.get("config.fullscreen", false);
	isSoundMuted = pt.get("config.muted", false);

	// Update the grid and window size
	gridSize = boardSize * GRID_SIZE_MULTIPLIER; // use increments of 8
	screenWidth = numberOfHorizontalTiles * gridSize; // 224 width with 8x8 tiles
	screenHeight = numberOfVerticalTiles * gridSize; // 288 height with 8x8 tiles

	return true;
}

bool Config::SaveConfig(std::string& filename)
{
	using boost::property_tree::ptree;

	ptree pt;
	pt.get("config.size", gridSize / GRID_SIZE_MULTIPLIER);
	pt.get("config.fullscreen", isFullScreen);
	pt.get("config.muted", isSoundMuted);

	write_xml(filename, pt);

	return false;
}

