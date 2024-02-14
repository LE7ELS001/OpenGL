#pragma once
#include "Renderer.h"
#include <iostream>


class G_buffer {
private:
	unsigned int m_RendererID;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedoSpec;
	unsigned int rboDepth;
	unsigned int SC_WIDTH = 960.0f;
	unsigned int SC_HEIGHT = 540.0f;
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 };
public:
	G_buffer();
	~G_buffer();

	unsigned int GetBufferID();

	void Bind();
	void UnBind();
	void BindPosition(unsigned int slot = 0) const;
	void BindNormal(unsigned int slot = 0) const;
	void BindAlbedo(unsigned int slot = 0) const;
};