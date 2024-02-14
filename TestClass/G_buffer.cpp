#include "G_buffer.h"

G_buffer::G_buffer()
{
	//生成g缓冲
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	//4个颜色附件 其中一个二合一了
	//位置
	GLCall(glGenTextures(1, &gPosition));
	GLCall(glBindTexture(GL_TEXTURE_2D, gPosition));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SC_WIDTH, SC_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0));

	//法向量
	GLCall(glGenTextures(1, &gNormal));
	GLCall(glBindTexture(GL_TEXTURE_2D, gNormal));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SC_WIDTH, SC_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0));

	//漫射（颜色）和高光
	GLCall(glGenTextures(1, &gAlbedoSpec));
	GLCall(glBindTexture(GL_TEXTURE_2D, gAlbedoSpec));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SC_WIDTH, SC_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0));

	GLCall(glDrawBuffers(3,attachments));

	//rboDepth
	GLCall(glGenRenderbuffers(1, &rboDepth));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SC_WIDTH, SC_HEIGHT));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth));

	//check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

G_buffer::~G_buffer()
{
}

unsigned int G_buffer::GetBufferID()
{
	return m_RendererID;
}

void G_buffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void G_buffer::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void G_buffer::BindPosition(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, gPosition);
}

void G_buffer::BindNormal(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, gNormal);
}

void G_buffer::BindAlbedo(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
}


