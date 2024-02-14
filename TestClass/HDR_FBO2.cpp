#include "HDR_FBO2.h"

HDR_FBO2::HDR_FBO2()
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	
	// ÑÕÉ«¸½¼þ
	GLCall(glGenTextures(2, ColorBuffers));
	for (unsigned int i = 0; i < 2; i++)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, ColorBuffers[i]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SC_WIDTH, SC_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorBuffers[i], 0));
	}
	
	//rbo
	GLCall(glGenRenderbuffers(1, &rboDepth));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SC_WIDTH, SC_HEIGHT));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	
	GLCall(glDrawBuffers(2, Attachment));
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	//glGenFramebuffers(1, &m_RendererID);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
	//unsigned int colorBuffers[2];
	//glGenTextures(2, colorBuffers);
	//for (unsigned int i = 0; i < 2; i++)
	//{
	//	glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SC_WIDTH, SC_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	// attach texture to framebuffer
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	//}
	//// create and attach depth buffer (renderbuffer)
	//
	//glGenRenderbuffers(1, &rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SC_WIDTH, SC_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	//// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	//unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	//glDrawBuffers(2, attachments);
	//// finally check if framebuffer is complete
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "Framebuffer not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

HDR_FBO2::~HDR_FBO2()
{
}

unsigned int HDR_FBO2::GetColor(unsigned int num)
{
	return ColorBuffers[num];
}

unsigned int HDR_FBO2::rbo()
{
	return rboDepth;
}

void HDR_FBO2::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void HDR_FBO2::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HDR_FBO2::BindColorBuffer(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_2D, ColorBuffers[slot]);
}
