#pragma once
#include "SDL.h"

class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	void Start();
	void Stop();
	void Pause();
	void Resume();
	Uint32 GetTicks();
	bool IsStarted() { return isStarted; }
	bool IsPaused() { return (isPaused && isStarted); }
private:
	Uint32 startTicks;
	Uint32 pausedTicks;
	bool isStarted;
	bool isPaused;
};