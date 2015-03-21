#include "Config.h"

namespace Config
{
	std::string debugLogFolder = "Logs/";
	int gridSize = BoardSizeEnum::Medium * GRID_SIZE_MULTIPLIER; // use increments of 8
	int numberOfHorizontalTiles = 28;
	int numberOfVerticalTiles = 36;
	int screenWidth = numberOfHorizontalTiles * gridSize; // 224 width with 8x8 tiles
	int screenHeight = numberOfVerticalTiles * gridSize; // 288 height with 8x8 tiles
	bool isFullScreen = false;
	bool isSoundMuted = false;
}

bool Config::LoadConfig(std::string& filename)
{
	using boost::property_tree::ptree;

	// Create a property tree
	ptree pt;

	// Load the XML into the tree
	read_xml(filename, pt);

	//file = pt.get<std::string>("config.filename");

	int boardSize = pt.get("config.size", 2);
	isFullScreen = pt.get("config.fullscreen", false);
	isSoundMuted = pt.get("config.muted", false);

	//std::string debugLogFolder = "Logs/";
	gridSize = boardSize * GRID_SIZE_MULTIPLIER; // use increments of 8
	numberOfHorizontalTiles = 28;
	numberOfVerticalTiles = 36;
	screenWidth = numberOfHorizontalTiles * gridSize; // 224 width with 8x8 tiles
	screenHeight = numberOfVerticalTiles * gridSize; // 288 height with 8x8 tiles

	return true;
}

bool Config::SaveConfig(std::string& filename)
{
	// Check if the configuration file exists
		// Create the configuration file

	// Store the config values in the XML file
	using boost::property_tree::ptree;

	boost::property_tree::ptree pt;
	//pt.put("config.filename", file);

	write_xml(filename, pt);

	return false;
}

