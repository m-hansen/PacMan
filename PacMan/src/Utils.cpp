#pragma once
#include "Utils.h"

bool Utils::CollisionChecker(SDL_Rect* a, SDL_Rect* b)
{
	// Set the bounds for collider A
	int leftA = a->x;
	int rightA = a->x + a->w;
	int topA = a->y;
	int bottomA = a->y + a->h;

	// Set the bounds for collider B
	int leftB = b->x;
	int rightB = b->x + b->w;
	int topB = b->y;
	int bottomB = b->y + b->h;

	// Check for collision
	// if any of these statements are true we know that there is a gap between 
	// the two colliders on the horizontal or vertical axis
	if ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB))
	{
		return false;
	}

	// Both the horizontal and vertical coordinates have an overlap
	return true;
}

void Utils::CreateFolder(std::string folderName)
{
	boost::filesystem::path directory(folderName);

	// Remove the old directory and all of its contents
	boost::filesystem::remove_all(directory);

	// Create the new directory
	boost::filesystem::create_directory(directory);
}

SDL_Texture* Utils::CreateFontTexture(SDL_Renderer* renderer, TTF_Font* fontType, std::string text, SDL_Color fontColor)
{
	// Create the controls font surface and texture
	SDL_Surface* fontSurface = TTF_RenderText_Solid(fontType, text.c_str(), fontColor);
	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;

	return fontTexture;
}

void Utils::RenderText(SDL_Renderer* renderer, TTF_Font* fontType, std::string text, SDL_Color fontColor,
	SDL_Rect* rect)
{
	// Create the controls font surface and texture
	SDL_Surface* fontSurface = TTF_RenderText_Solid(fontType, text.c_str(), fontColor);
	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = NULL;
	SDL_RenderCopy(renderer, fontTexture, NULL, rect);
	SDL_DestroyTexture(fontTexture);
	fontTexture = NULL;
}

void Utils::RenderText(SDL_Renderer* renderer, TTF_Font* fontType, std::string text, SDL_Color fontColor,
	float x, float y, float w, float h)
{
	// Create the bounding rectangle
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	RenderText(renderer, fontType, text, fontColor, &rect);
}

void Utils::SwapPointers(int* a, int* b)
{
	*a += *b;
	*b = *a - *b;
	*a = *a - *b;
}

void Utils::Encrypt(std::string& data, std::string key)
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i] += key[i % key.size()];
	}
}

void Utils::Decrypt(std::string& data, std::string key)
{
	for (unsigned i = 0; i < data.size(); i++)
	{
		data[i] -= key[i % key.size()];
	}
}

void Utils::LoadMixChunk(Mix_Chunk** chunk, std::string pathWithExtension)
{
	*chunk = Mix_LoadWAV(pathWithExtension.c_str());
	if (&chunk == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
}