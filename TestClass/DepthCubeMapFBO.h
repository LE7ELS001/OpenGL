#pragma once
#include "Renderer.h"
#include <iostream>

class DepthCubeMapFBO {
private:
	unsigned int m_RendererID;
	unsigned int depthCubeMap;
	const int SHADOW_WIDTH = 1024;
	const int SHADOW_HEIGHT = 1024;
public:
	DepthCubeMapFBO();
	~DepthCubeMapFBO();

	void Bind();
	void UnBind();
	void BindDepthMap(unsigned int slot = 0) const;
};