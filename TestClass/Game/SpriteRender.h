#pragma once
#include "Renderer.h"
#include <iostream>
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "TestClass/Game/ResourceManager.h"
#include "TestClass/Game/BrickTexture.h"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class SpriteRender
{
private:

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	
	
	void initRenderData();

public :
	SpriteRender();
	~SpriteRender();

	//void DrawSprite(const std::unique_ptr<Texture>& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	void DrawSprite(const std::unique_ptr<Shader> &shader, const std::unique_ptr<Texture> &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	void DrawSpriteBrick(const std::unique_ptr<Shader>& shader,BrickTexture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	

	//============设置相关参数=================
	//正方形参数
	float vertices[24] =
	{
		// 位置      纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	//模型矩阵
	glm::mat4 model;
};
