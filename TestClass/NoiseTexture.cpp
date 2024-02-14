#include "TestClass/NoiseTexture.h"

NoiseTexture::NoiseTexture(std::vector<glm::vec3> noise_tex)
{
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise_tex[0]));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S ,GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

NoiseTexture::~NoiseTexture()
{
}

void NoiseTexture::BindNoiseTexture(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
