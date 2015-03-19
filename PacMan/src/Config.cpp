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

/*bool Config::LoadConfig(std::string& filename)
{
	using boost::property_tree::ptree;

	// Create a property tree
	ptree pt;

	// Load the XML into the tree
	read_xml(filename, pt);

	file = pt.get<std::string>("config.filename");

	BOOST_FOREACH(ptree::value_type& v, pt.get_child)
	return false;
}

bool Config::SaveConfig(std::string& filename)
{
	// Check if the configuration file exists
		// Create the configuration file

	// Store the config values in the XML file
	using boost::property_tree::ptree;

	boost::property_tree::ptree pt;
	pt.put("config.filename", file);
	
	boost::property_tree::ptree BOOST_FOREACH(const std::string& name, modules)
	{
		pt.add("config.modules.module", name);
	}

	write_xml(filename, pt);

	return false;
}*/

