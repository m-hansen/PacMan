//#pragma once
//#include <SDL.h>
//#include "TextureManager.h"
//#include "Utils.h"
//#include "TileTypeEnum.h"
//
//class Tile
//{
//public:
//	Tile(int x, int y);
//	Tile(Vector2 pos);
//	~Tile();
//	void Tile::Initialize();
//	void Render(SDL_Renderer* renderer);
//	TileTypeEnum* GetContents();
//	void SetContents(TileTypeEnum type);
//	void SetTexture(SDL_Texture* texture);
//	Vector2 GetPosition();
//	SDL_Rect* GetBoundingRect();
//	int GetId();
//private:
//	int uniqueId;
//	int tileSize;
//	Vector2 position;
//	SDL_Rect boundingRect;
//	SDL_Texture* tileTexture;
//	TileTypeEnum contents;
//};