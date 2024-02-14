#pragma once

#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "TestClass/GameBreakout/GameBObject.h"
#include "GameTexture.h"

class BallObject : public GameBOject
{
public :
	float Radius; //球半径
	bool Stuck;   //是否卡在平台上
	bool Sticky, PassThrough; //后期一些特殊道具效果

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, GameTexture sprite);

	//球的移动检测
	glm::vec2 Move(float dt, unsigned int window_width);

	//重置球的位置（通关或者没接住）
	void Reset(glm::vec2 position, glm::vec2 velocity);
};