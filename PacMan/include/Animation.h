#include "SDL.h"

enum AnimationState
{
	Playing,
	Stopped,
	Paused
};

class Animation
{
public:
	Animation(SDL_Texture* sprites);
	~Animation();
	void Render(SDL_Renderer* renderer, SDL_Rect* renderTarget);
	void Play();
	void Stop();
	void Pause();
	SDL_Rect& GetCurrentClip() { return *currentClip; }
private:
	AnimationState controlState;
	SDL_Rect spriteClips[100];
	SDL_Rect* currentClip;
	SDL_Texture* spriteSheet;
	int frame;
	int totalNumberOfFrames;
	// TODO animation cycle speed?
};