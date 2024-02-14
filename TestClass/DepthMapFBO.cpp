#include "DepthMapFBO.h"

DepthMapFBO::DepthMapFBO()
{
	//生成framebuffer
	GLCall(glGenFramebuffers(1, &m_RendererID));
	
	//生成深度贴图纹理
	GLCall(glGenTextures(1, &depthMap));
	GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	
	//设置CLAMP_TO_BORDER是因为在裁剪外的地方 需要默认为无阴影 不然用repeat会默认为是阴影
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	//float borderColor[] = { 1.0,1.0,1.0,1.0 };
	//GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));
	
	//只需要深度的数值 不需要color
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
