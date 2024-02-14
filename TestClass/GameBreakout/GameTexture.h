#pragma once
#include <iostream>

#include "GL/glew.h"

class GameTexture
{
public :
	unsigned int TextureID;
	unsigned int Width, Height;
	unsigned int Internal_format; //内部格式
	unsigned int Image_format; //图片格式

	GameTexture();

	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	void Bind() const;
};
