#pragma once
#include "Renderer.h"
#include <iostream>


class FBO_SSAO {
private:
	unsigned int m_RendererID;
	unsigned int SSAO_RT;
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;
	
public:
	FBO_SSAO();
	~FBO_SSAO();


	void Bind();
	void UnBind();

	void BindSSAO_RT(unsigned int slot = 0) const;

	
};