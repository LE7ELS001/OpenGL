#include "TestClass/FBO_IBL_D.h"

FBO_IBL_D::FBO_IBL_D()
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

	//envCubeMap
	GLCall(glGenTextures(1, &envCubeMap));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap));

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));


	//IrradianceMap
	GLCall(glGenTextures(1, &irradianceMap));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap));

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//prefilterMap
	GLCall(glGenTextures(1, &prefilterMap));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap));

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	

	//BRDF
	GLCall(glGenTextures(1, &BRDF));
	GLCall(glBindTexture(GL_TEXTURE_2D, BRDF));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//rbo
	GLCall(glGenRenderbuffers(1, &m_rbo));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo));
}

FBO_IBL_D::~FBO_IBL_D()
{
}

void FBO_IBL_D::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FBO_IBL_D::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FBO_IBL_D::Bindrbo()
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
}

void FBO_IBL_D::BindEnvCubeMap()
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap));
}

void FBO_IBL_D::BindIrradianceMap()
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap));
}

void FBO_IBL_D::BindPrefilterMap()
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap));
}

void FBO_IBL_D::BindBRDF()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, BRDF));
}

unsigned int FBO_IBL_D::envCubeMapNumber()
{
	return envCubeMap;
}

unsigned int FBO_IBL_D::irradianceMapNumber()
{
	return irradianceMap;
}

unsigned int FBO_IBL_D::prefilterMapNumber()
{
	return prefilterMap;
}

unsigned int FBO_IBL_D::BRDFNumber()
{
	return BRDF;
}
