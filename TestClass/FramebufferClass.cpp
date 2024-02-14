#include "FramebufferClass.h"

FrameBuffer::FrameBuffer()
{
	//帧缓冲

	//创建帧缓冲
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	//创建纹理附件 可取用里面的数值
	GLCall(glGenTextures(1, &textureBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureBuffer));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 540, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	//参数 （缓冲目标， 附件类型， 附加的纹理类型， data， 多级渐远纹理级别）
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0));
	
	//创建渲染缓冲对象附件（通常是只写） 因为它使用原生的OPENGL渲染格式 所以缓冲到缓冲之间传递是有优化的
	GLCall(glGenRenderbuffers(1, &rbo));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 540)); //渲染缓冲对象附件并不适用通用数据缓冲 因此要选择内部格式 所以是GL_DEPTH24_STENCIL8 意味着24位装深度 8位装模板
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
	//检查是否加入附件完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER IS NOT COMPLETE" << std::endl;
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	
	//glGenFramebuffers(1, &m_RendererID);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//
	//glGenTextures(1, &textureBuffer);
	//glBindTexture(GL_TEXTURE_2D, textureBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 540, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
	//// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 540); // use a single renderbuffer object for both a depth AND stencil buffer.
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	//// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
}

FrameBuffer::~FrameBuffer()
{
}

unsigned int  FrameBuffer::GetFrame()
{
	return m_RendererID;
}

unsigned int FrameBuffer::GetTexture()
{
	return textureBuffer;
}

unsigned int FrameBuffer::GetRBO()
{
	return rbo;
}

void FrameBuffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::BindTexture()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, textureBuffer));
}

