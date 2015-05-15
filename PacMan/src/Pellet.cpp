#include "Pellet.h"

Pellet::Pellet(Node* node)
{
	
	location = node;
	type = node->GetType();
	isVisible = true;

	if (type == NodeTypeEnum::PelletNode)
	{
		value = 10;
		texture = TextureManager::GetTexture("pellet");
		InitBoundingRect(Config::gridSize / 4);
		// Note: if the normal pellets need to blink then the blink timer should be started first
	}
	else if (type == NodeTypeEnum::PowerPelletNode)
	{
		value = 50;
		texture = TextureManager::GetTexture("powerPellet");
		InitBoundingRect(Config::gridSize / 2);
		blinkTimer.Start();
	}
	else
		fprintf(stdout, "ERROR: %s is not a valid pellet type\n", std::to_string(type));
}

Pellet::~Pellet()
{

}

void Pellet::InitBoundingRect(int size)
{
	boundingRect.w = size;
	boundingRect.h = size;
	boundingRect.x = (location->GetPosition().x) + (Config::gridSize / 2 - size / 2);
	boundingRect.y = (location->GetPosition().y) + (Config::gridSize / 2 - size / 2);
}

void Pellet::PlayBlinkAnimation(const int BLINK_RATE)
{
	// Reminder: this function is intended to be used with power pellets
	// if standard pellets need to blink, then a blink timer should be started first

	if (blinkTimer.GetTicks() > BLINK_RATE)
	{
		isVisible = !isVisible;
		blinkTimer.Start(); // resets the timer
	}
}

void Pellet::Render(SDL_Renderer* renderer)
{
	if (!isVisible) return;
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);
}