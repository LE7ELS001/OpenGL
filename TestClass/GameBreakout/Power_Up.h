#pragma once
#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "TestClass/GameBreakout/GameBObject.h"

#include <string>

/*
	��Ϸ�������
*/

//����ͼ��Ĵ�С
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);

//���������ٶ�
const glm::vec2 VELOCITY(0.0f, 150.0f);

class Power_Up : public GameBOject
{
public:
	std::string Type; //��������
	float Duration;	  //���߳���ʱ��
	bool Activated;   //�����Ƿ񱻼���

	Power_Up(std::string type, glm::vec3 color, float duration, glm::vec2 position, GameTexture texture)
		: GameBOject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated()
	{

	}
};
