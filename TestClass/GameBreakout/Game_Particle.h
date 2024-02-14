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
	float Life; //生命周期

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
	
	//初始化
	void Init();

	//查找获取生命周期结束的粒子
	unsigned int firstUnusedParticle();

	//重置粒子各项属性
	void ResetParticle(Particle& particle, GameBOject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
