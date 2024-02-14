#include "FramebufferClass.h"

FrameBuffer::FrameBuffer()
{
	//֡����

	//����֡����
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	//���������� ��ȡ���������ֵ
	GLCall(glGenTextures(1, &textureBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureBuffer));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 540, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	//���� ������Ŀ�꣬ �������ͣ� ���ӵ��������ͣ� data�� �༶��Զ������
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0));
	
	//������Ⱦ������󸽼���ͨ����ֻд�� ��Ϊ��ʹ��ԭ����OPENGL��Ⱦ��ʽ ���Ի��嵽����֮�䴫�������Ż���
	GLCall(glGenRenderbuffers(1, &rbo));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 540)); //��Ⱦ������󸽼���������ͨ�����ݻ��� ���Ҫѡ���ڲ���ʽ ������GL_DEPTH24_STENCIL8 ��ζ��24λװ��� 8λװģ��
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
	//����Ƿ���븽������
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

