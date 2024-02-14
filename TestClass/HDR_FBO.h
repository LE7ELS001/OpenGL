#pragma once
#include "Renderer.h"
#include <iostream>

class HDR_FBO {
private:
	unsigned int m_RendererID;
	unsigned int ColorBuffer;
	unsigned int rbo;
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;
public:
	HDR_FBO();
	~HDR_FBO();

	unsigned int GetColor();

	void Bind();
	void UnBind();
	void BindColorBuffer(unsigned int slot = 0) const;
};