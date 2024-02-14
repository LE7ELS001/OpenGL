#pragma once
#include "Renderer.h"
#include <iostream>

class FrameBuffer{
private:
	unsigned int m_RendererID;
	unsigned int textureBuffer;
	unsigned int rbo;
public:
	FrameBuffer();
	~FrameBuffer();

	unsigned int GetFrame();
	unsigned int GetTexture();
	unsigned int GetRBO();


	void Bind();
	void UnBind();
	void BindTexture();
};