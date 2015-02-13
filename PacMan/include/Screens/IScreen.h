#pragma once
#include "SDL.h"
#include "Global.h"

class IScreen
{
public:
	virtual ~IScreen() {};
	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};