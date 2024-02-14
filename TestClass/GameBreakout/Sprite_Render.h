#pragma once
#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

#include "GameTexture.h"
#include "GameShader.h"

class Sprite_Render
{
public:
	Sprite_Render(GameShader& shader);
	~Sprite_Render();

	void DrawSprtie(GameTexture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	GameShader m_shader;
	unsigned int quadVAO;

	void initRenderData();
};
