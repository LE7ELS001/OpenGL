#pragma once
#include "Renderer.h"
#include <iostream>


class FBO_SSAO_Blur {
private:
	unsigned int m_RendererID;
	unsigned int SSAO_Blur_RT;
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;

public:
	FBO_SSAO_Blur();
	~FBO_SSAO_Blur();


	void Bind();
	void UnBind();

	void BindBlurRt(unsigned int slot = 0) const;

};
