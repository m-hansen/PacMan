#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (std::map<std::string,SDL_Texture*>::iterator iter = textureMap.begin(); iter != textureMap.end(); ++iter)
	{
		if (iter->second != NULL) 
		{
			// Destroy each texture in the map
			SDL_DestroyTexture(iter->second);
		}
	}
}

bool TextureManager::LoadTexture(SDL_Renderer* renderer, std::string referenceName, std::string location)
{
	SDL_Surface* surface = NULL;

	if (location.substr(location.length()-3, location.length()) == "bmp")
	{
		surface = SDL_LoadBMP(location.c_str());
		if (!surface)
		{
			printf("Could not load bitmap! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}
	else
	{
		surface = IMG_Load(location.c_str());
		if (!surface)
		{
			printf("Could not load image! IMG_Error: %s\n", IMG_GetError());
			return false;
		}
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Create and add to map if successful
	textureMap.insert(std::map<std::string, SDL_Texture*>::value_type(referenceName.c_str(), texture));
	SDL_FreeSurface(surface);

	return true;
}

SDL_Texture* TextureManager::GetTexture(std::string key)
{
	return textureMap.find(key)->second;
}

