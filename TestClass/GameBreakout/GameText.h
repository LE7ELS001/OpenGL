#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Vendor/ft2build.h"
#include FT_FREETYPE_H

#include "Resource_manager.h"
#include "GameShader.h"
#include "GameTexture.h"

#include <iostream>
#include <map>

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class GameText
{
public :
	std::map<char, Character> Characters;

	GameShader TextShader;

	GameText(unsigned int width, unsigned int height);

	void Load(std::string font , unsigned int fontSize);

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private :
	unsigned int VAO, VBO;
};
