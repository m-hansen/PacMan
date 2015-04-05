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
	static void CreateFolder(std::string folderName);
	static SDL_Texture* CreateFontTexture(SDL_Renderer* renderer, TTF_Font* fontType, std::string text, SDL_Color fontColor);
	static void RenderText(SDL_Renderer* renderer, TTF_Font* fontType, std::string text, SDL_Color fontColor,
		SDL_Rect* rect);
	static void RenderText(SDL_Renderer* renderer, TTF_Font* fontType, std::string text, SDL_Color fontColor,
		float x, float y, float w, float h);
	static void SwapPointers(int* a, int* b);
	static void Encrypt(std::string& data, std::string key);
	static void Decrypt(std::string& data, std::string key);
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