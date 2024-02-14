#include "HDR_FBO.h"

HDR_FBO::HDR_FBO()
{
	////浮点帧缓冲
	//GLCall(glGenFramebuffers(1, &m_RendererID));
	//
	////颜色附件
	//GLCall(glGenTextures(1, &ColorBuffer));
	//GLCall(glBindTexture(GL_TEXTURE_2D, ColorBuffer));
	//GLCall(glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,SC_WIDTH, SC_HEIGHT,0,GL_RGBA,GL_FLOAT,NULL));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//
	////rbo
	//GLCall(glGenRenderbuffers(1, &rbo));
	//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SC_WIDTH, SC_HEIGHT);
	//
	////链接上面生成的附件
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer, 0));
	//GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo));
	////检查是否加入附件完整
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cout << "ERROR::FRAMEBUFFER IS NOT COMPLETE" << std::endl;
	//}
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	glGenFramebuffers(1, &m_RendererID);
	
	glGenTextures(1, &ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SC_WIDTH, SC_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SC_WIDTH, SC_HEIGHT);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

HDR_FBO::~HDR_FBO()
{
}

unsigned int HDR_FBO::GetColor()
{
	return ColorBuffer;
}

void HDR_FBO::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void HDR_FBO::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HDR_FBO::BindColorBuffer(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, ColorBuffer);
}

