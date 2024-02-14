#include "TestClass/DepthCubeMapFBO.h""

DepthCubeMapFBO::DepthCubeMapFBO()
{
	//生成帧缓冲
	GLCall(glGenFramebuffers(1, &m_RendererID));
	
	//生成cubemap的材质
	GLCall(glGenTextures(1, &depthCubeMap));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap));
	
	//设定六个方向面
	for (unsigned int i = 0; i < 6; i++)
	{
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	}
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	
	//绑定贴图到帧缓冲附件
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0));
	
	//只需要立方体材质贴图 颜色不需要
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	
	//glGenFramebuffers(1, &m_RendererID);
	//// create depth cubemap texture
	//
	//glGenTextures(1, &depthCubeMap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	//for (unsigned int i = 0; i < 6; ++i)
	//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//// attach depth texture as FBO's depth buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthCubeMapFBO::~DepthCubeMapFBO()
{
}

void DepthCubeMapFBO::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void DepthCubeMapFBO::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthCubeMapFBO::BindDepthMap(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
}
