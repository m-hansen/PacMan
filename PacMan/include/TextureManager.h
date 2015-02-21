#pragma once
#include <string>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

class TextureManager
{
public:
	static bool LoadTexture(SDL_Renderer* renderer, std::string referenceName, std::string location);
	static SDL_Texture* GetTexture(std::string key);
	static SDL_Texture* CreateFontTexture(SDL_Renderer* renderer, std::string text,
		TTF_Font* fontType, SDL_Color fontColor);
	static void UnloadTexture(std::string referenceName);
	static void ClearAll();
private:
	static std::map<std::string, SDL_Texture*> textureMap;
};