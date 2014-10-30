#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "Sprite.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	bool LoadTexture(SDL_Renderer* renderer, std::string referenceName, std::string location);
	SDL_Texture* GetTexture(std::string key);
private:
	std::map<std::string, SDL_Texture*> textureMap;
};