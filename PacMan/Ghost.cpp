#include "Ghost.h"

Ghost::Ghost(std::string textureName)
{
	texture = TextureManager::GetTexture(textureName.c_str());
	position.x = 7 * G_SIZE;
	position.y = 5 * G_SIZE;

	boundingRect.w = G_SIZE;
	boundingRect.h = G_SIZE;
	speed = 0.05f * (G_SIZE / 8);
	direction = DirectionEnum::Left;
	previousDirection = direction;

	currentNode = NULL;
	previousNode = NULL;
}

Ghost::~Ghost()
{
	// Do not free the texture here!
	// The TextureManager will handle this
	texture = NULL;
}

void Ghost::Update(Uint32 deltaT)
{
	// Update the position
	switch (direction)
	{
	case Up:
		position.y -= speed * deltaT;
		break;
	case Down:
		position.y += speed * deltaT;
		break;
	case Left:
		position.x -= speed * deltaT;
		break;
	case Right:
		position.x += speed * deltaT;
		break;
	case None:
		break;
	}

	boundingRect.x = position.x - (G_SIZE / 2);
	boundingRect.y = position.y - (G_SIZE / 2);
}

void Ghost::UpdateNodes(Node* newNode)
{
	previousNode = currentNode;
	currentNode = newNode;
}

void Ghost::Render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &boundingRect);
}

void Ghost::SetDirection(DirectionEnum dirEnum)
{
	previousDirection = direction;
	direction = dirEnum;
}

Node* Ghost::GetCurrentNode()
{
	return currentNode;
}

Node* Ghost::GetPreviousNode()
{
	return previousNode;
}

DirectionEnum Ghost::GetDirection()
{
	return direction;
}

void Ghost::SetPosition(Node* node)
{
	position.x = (G_SIZE * node->GetLocation().x) + (G_SIZE / 2);
	position.y = (G_SIZE * node->GetLocation().y) + (G_SIZE / 2);
	currentNode = node;
}
SDL_Rect* Ghost::GetBoundingRect()
{
	return &boundingRect;
}

void Ghost::SetPreviousDirection(DirectionEnum dir)
{
	previousDirection = dir;
}