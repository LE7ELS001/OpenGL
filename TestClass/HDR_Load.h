#pragma once
#include "Renderer.h"
#include <iostream>
#include "Texture.h"
#include "Vendor/STB/stb_image.h"


class HDR_Load
{
private:
	unsigned int  m_RendererID;
	std::string m_FilePath; //Í¼Æ¬Â·¾¶
	float* data;
	int m_Width, m_Height, m_BPP;

	//ÊµÑé



public:
	HDR_Load(const std::string& path);
	~HDR_Load();

	void Bind(unsigned int slot = 0) const;
	void UnBind();

	unsigned int HDRTextureNumber();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }





};