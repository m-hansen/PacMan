#pragma once
#include <direct.h>
#include <string>
#include "SDL.h"
#include "boost/filesystem.hpp"

struct Vector2
{
	int x;
	int y;
};

struct Vector2f
{
	float x;
	float y;
};

class Utils
{
public:
	static bool CollisionChecker(SDL_Rect* a, SDL_Rect* b);
	static void CreateFolder(std::string folderName);
private:
	Utils();
};