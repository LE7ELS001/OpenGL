#pragma once

#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "TestClass/GameBreakout/GameShader.h"
#include "TestClass/GameBreakout/GameTexture.h"	
#include "TestClass/GameBreakout/GameBObject.h"

#include <vector>

struct Particle
{
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life; //��������

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class Game_Particle
{
public:
	Game_Particle(GameShader shader, GameTexture texture, unsigned int amount);


	void Update(float dt, GameBOject& object, unsigned int newParticles, glm::vec2 offset);

	void Draw();

private:
	GameShader m_shader;
	GameTexture m_texture;
	unsigned int Amount;
	unsigned int VAO;
	
	std::vector<Particle> particles;
	
	//��ʼ��
	void Init();

	//���һ�ȡ�������ڽ���������
	unsigned int firstUnusedParticle();

	//�������Ӹ�������
	void ResetParticle(Particle& particle, GameBOject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
