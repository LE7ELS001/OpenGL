#pragma once
#include "Renderer.h"
#include <iostream>

class MASS_FrameBuffer {
private:
	unsigned int m_RendererID;
	unsigned int textureBuffer;
	unsigned int rbo;

public:
	MASS_FrameBuffer();
	~MASS_FrameBuffer();

	unsigned int GetFrame();
	unsigned int GetTexture();
	unsigned int GetRBO();

	void Bind();
	void UnBind();
	void BindTexture();
};