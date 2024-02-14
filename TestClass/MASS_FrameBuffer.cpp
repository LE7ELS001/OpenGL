#include "TestClass/MASS_FrameBuffer.h"

MASS_FrameBuffer::MASS_FrameBuffer()
{
	////多重采样帧缓冲
	//GLCall(glGenFramebuffers(1, &m_RendererID));
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	//
	////多重采样附件
	//GLCall(glGenTextures(1, &textureBuffer));
	//GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBuffer));
	//GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 900, 540, GL_TRUE));
	//GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
	//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureBuffer, 0));
	//
	////RBO
	//GLCall(glGenRenderbuffers(1, &rbo));
	//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	//GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 900, 540));
	//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	//GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
	//
	////检查是否有附件绑定到帧缓冲
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cout << "ERROR::Framebuffer is incomplete" << std::endl;
	//}

	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	// create a multisampled color attachment texture

	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 900, 540, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureBuffer, 0);
	// create a (also multisampled) renderbuffer object for depth and stencil attachments

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 900, 540);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	
	


	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

MASS_FrameBuffer::~MASS_FrameBuffer()
{
}

unsigned int MASS_FrameBuffer::GetFrame()
{
	return m_RendererID;
}

unsigned int MASS_FrameBuffer::GetTexture()
{
	return textureBuffer;
}

unsigned int MASS_FrameBuffer::GetRBO()
{
	return rbo;
}

void MASS_FrameBuffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void MASS_FrameBuffer::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void MASS_FrameBuffer::BindTexture()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, textureBuffer));
}
