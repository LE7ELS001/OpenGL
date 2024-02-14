#pragma once
#include "Renderer.h"
#include <iostream>


class FBO_IBL_D {
private:
	unsigned int m_RendererID;
	unsigned int m_rbo;
	unsigned int envCubeMap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int BRDF;
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;

public:
	FBO_IBL_D();
	~FBO_IBL_D();


	void Bind();
	void UnBind();
	void Bindrbo();
	void BindEnvCubeMap();
	void BindIrradianceMap();
	void BindPrefilterMap();
	void BindBRDF();
	
	unsigned int envCubeMapNumber();
	unsigned int irradianceMapNumber();
	unsigned int prefilterMapNumber();
	unsigned int BRDFNumber();
};