#pragma once
#include <iostream>

#include "GL/glew.h"

class GameTexture
{
public :
	unsigned int TextureID;
	unsigned int Width, Height;
	unsigned int Internal_format; //�ڲ���ʽ
	unsigned int Image_format; //ͼƬ��ʽ

	GameTexture();

	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	void Bind() const;
};
