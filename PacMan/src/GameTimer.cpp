#include "GameTimer.h"

GameTimer::GameTimer()
{
	startTicks = 0;
	pausedTicks = 0;
	isStarted = false;
	isPaused = false;
}

GameTimer::~GameTimer()
{

}

void GameTimer::Start()
{
	// Set the flags
	isStarted = true;
	isPaused = false;

	// Get the time
	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

void GameTimer::Stop()
{
	// Reset the flags
	isStarted = false;
	isPaused = false;

	// Clear the time
	startTicks = 0;
	pausedTicks = 0;
}

void GameTimer::Pause()
{
	if (isStarted && !isPaused)
	{
		isPaused = true;

		// Get the time paused
		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void GameTimer::Resume()
{
	if (isStarted && isPaused)
	{
		isPaused = false;

		// Get the start ticks excluding the time passed while paused
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

Uint32 GameTimer::GetTicks()
{
	// The actual time on the timer
	Uint32 time = 0;

	if (isStarted)
	{
		if (isPaused)
		{
			// Get the number of ticks while paused
			time = pausedTicks;
		}
		else
		{
			// Get the current time minus the start time
			time = SDL_GetTicks() - startTicks;
		}
	}

	return time;
}