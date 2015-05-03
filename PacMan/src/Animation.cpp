#include "Animation.h"

Animation::Animation(SDL_Texture* sprites) : 
	spriteSheet(sprites), frame(0), controlState(Playing)
{
}

Animation::~Animation()
{
}

void Animation::Play()
{
	controlState = AnimationState::Playing;
}

void Animation::Stop()
{
	controlState = AnimationState::Stopped;
	frame = 0;
}

void Animation::Pause()
{
	controlState = AnimationState::Paused;
}

void Animation::Render(SDL_Renderer* renderer, SDL_Rect* renderTarget)
{
	// Render the current frame of the animation
	currentClip = &spriteClips[frame / totalNumberOfFrames];
	SDL_RenderCopy(renderer, spriteSheet, currentClip, renderTarget);

	// Update the screen
	SDL_RenderPresent(renderer);

	if (controlState == AnimationState::Playing)
	{
		// Move on to the next frame
		frame++;

		// Cycle the animation
		if (frame / totalNumberOfFrames >= totalNumberOfFrames)
		{
			frame = 0;
		}
	}
}