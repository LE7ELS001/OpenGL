#pragma once
#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "TestClass/GameBreakout/GameBObject.h"

#include <string>

/*
	游戏道具相关
*/

//道具图标的大小
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);

//道具下落速度
const glm::vec2 VELOCITY(0.0f, 150.0f);

class Power_Up : public GameBOject
{
public:
	std::string Type; //道具名字
	float Duration;	  //道具持续时间
	bool Activated;   //道具是否被激活

	Power_Up(std::string type, glm::vec3 color, float duration, glm::vec2 position, GameTexture texture)
		: GameBOject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated()
	{

	}
};
