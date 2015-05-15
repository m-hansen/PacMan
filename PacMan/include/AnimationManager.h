#include <string>
#include <map>
#include "Animation.h"

class AnimationManager
{
public:
	static void LoadAnimation(std::string key, Animation* val);
	static Animation* GetAnimation(std::string key);
private:
	static std::map<std::string, Animation*> animations;
};