#include "AnimationManager.h"

std::map<std::string, Animation*> AnimationManager::animations;

void AnimationManager::LoadAnimation(std::string key, Animation* val)
{
	animations.emplace(key, val);
}

Animation* AnimationManager::GetAnimation(std::string key)
{
	return animations.find(key)->second;
}