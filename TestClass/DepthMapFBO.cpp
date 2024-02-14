#include "DepthMapFBO.h"

DepthMapFBO::DepthMapFBO()
{
	//����framebuffer
	GLCall(glGenFramebuffers(1, &m_RendererID));
	
	//���������ͼ����
	GLCall(glGenTextures(1, &depthMap));
	GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	
	//����CLAMP_TO_BORDER����Ϊ�ڲü���ĵط� ��ҪĬ��Ϊ����Ӱ ��Ȼ��repeat��Ĭ��Ϊ����Ӱ
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	//float borderColor[] = { 1.0,1.0,1.0,1.0 };
	//GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));
	
	//ֻ��Ҫ��ȵ���ֵ ����Ҫcolor
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));




}

DepthMapFBO::~DepthMapFBO()
{
}

void DepthMapFBO::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void DepthMapFBO::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthMapFBO::BindDepthMap(unsigned int slot) const 
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
}
