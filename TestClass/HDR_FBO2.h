#pragma once
#include "Renderer.h"
#include <iostream>

class HDR_FBO2 {
private:
	unsigned int m_RendererID;
	unsigned int rboDepth;
	unsigned int ColorBuffers[2];
	unsigned int Attachment[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;
public:
	HDR_FBO2();
	~HDR_FBO2();

	unsigned int GetColor(unsigned int num);
	unsigned int rbo();

	void Bind();
	void UnBind();
	void BindColorBuffer(unsigned int slot = 0) const;
};