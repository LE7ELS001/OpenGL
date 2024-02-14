#pragma once

#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "TestClass/GameBreakout/GameBObject.h"
#include "GameTexture.h"

class BallObject : public GameBOject
{
public :
	float Radius; //��뾶
	bool Stuck;   //�Ƿ���ƽ̨��
	bool Sticky, PassThrough; //����һЩ�������Ч��

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, GameTexture sprite);

	//����ƶ����
	glm::vec2 Move(float dt, unsigned int window_width);

	//�������λ�ã�ͨ�ػ���û��ס��
	void Reset(glm::vec2 position, glm::vec2 velocity);
};