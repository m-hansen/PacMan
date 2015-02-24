#pragma once
#include <direct.h>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
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
	static SDL_Texture* Utils::CreateFontTexture(SDL_Renderer* renderer, TTF_Font* fontType, 
		std::string text, SDL_Color fontColor);
	static void CreateFolder(std::string folderName);
private:
	Utils();
};

class Timer
{
private:
	Uint32 startTime = 0;
public:
	void Pause();
	void Start();
	void Stop();
	void Reset();
};