#pragma once
#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "GameTexture.h"
#include "Sprite_Render.h"
#include "GameShader.h"

#include <iostream>

class Post_process
{
public:
	GameShader m_Shader;
	GameTexture m_Texture;
	unsigned int Width, Height;

	bool Confuse, Chaos, Shake;

	Post_process(GameShader shader, unsigned int width, unsigned int height);

	//渲染场景前
	void BeginRender();
	
	//渲染场景后
	void EndRender();

	//绘制到屏幕
	void Render(float time);

private:

	unsigned int MSFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;

	void initRenderData();
};
