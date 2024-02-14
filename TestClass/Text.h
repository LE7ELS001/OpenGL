#pragma once
#include <GL/glew.h>

#include <Vendor/glm/glm.hpp>
#include <Vendor/glm/gtc/matrix_transform.hpp>
#include <Vendor/glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <map>
#include <string>

class Text 
{
public:
	//字符构造体
	struct Character 
	{
		unsigned int TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	//窗口尺寸
	const int Width = 960;
	const int Height = 540;

	Text();
	~Text();

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);


private:
	Character m_character;

	unsigned int m_texture;


	//存储所有字符
	std::map<char, Character> Characters;

	//Shader
	Shader m_Shader;
	//shader相关的projection
	glm::mat4 m_pro;

	//VAO VBO
	unsigned int VAO;
	unsigned int VBO;



};
