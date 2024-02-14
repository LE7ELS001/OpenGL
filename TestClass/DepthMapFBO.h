#pragma once
#include "Renderer.h"
#include <iostream>

class DepthMapFBO {
private:
	unsigned int m_RendererID;
	unsigned int depthMap;
	const int SHADOW_WIDTH = 1024;
	const int SHADOW_HEIGHT = 1024;
public:
	DepthMapFBO();
	~DepthMapFBO();

	void Bind();
	void UnBind();
	void BindDepthMap(unsigned int slot = 0) const;
};