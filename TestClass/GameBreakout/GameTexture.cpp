#include "GameTexture.h"

GameTexture::GameTexture()
	:Width(0),Height(0), Internal_format(GL_RGB), Image_format(GL_RGB)
{
	glGenTextures(1, &this->TextureID);
}

void GameTexture::Generate(unsigned int width, unsigned int height,unsigned char* data)
{
	this->Width = width;
	this->Height = height;

	glBindTexture(GL_TEXTURE_2D, this->TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_format, width, height, 0, this->Image_format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameTexture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->TextureID);
}
