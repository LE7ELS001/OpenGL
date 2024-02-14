#include "TestClass/FBO_SSAO_Blur.h"

FBO_SSAO_Blur::FBO_SSAO_Blur()
{
	//����ģ����֡����
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	//��������
	GLCall(glGenTextures(1, &SSAO_Blur_RT));
	GLCall(glBindTexture(GL_TEXTURE_2D, SSAO_Blur_RT));

	//������ͼ
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SC_WIDTH, SC_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));

	//��ͼ����
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	//�󶨸���
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAO_Blur_RT, 0));

	//���
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO_SSAO_Blur::~FBO_SSAO_Blur()
{
}

void FBO_SSAO_Blur::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FBO_SSAO_Blur::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FBO_SSAO_Blur::BindBlurRt(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, SSAO_Blur_RT);
}
