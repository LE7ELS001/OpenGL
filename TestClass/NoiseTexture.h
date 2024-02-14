#pragma once
#include "Renderer.h"
#include <iostream>

class NoiseTexture
{
private:
	unsigned int  m_RendererID;


	//สตั้



public:
	NoiseTexture(std::vector<glm::vec3> noise_tex);
	~NoiseTexture();

	void BindNoiseTexture(unsigned int slot = 0) const;

	//void Bind(unsigned int slot = 0) const;
	//void UnBind();

};
