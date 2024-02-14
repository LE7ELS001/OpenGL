#pragma once

#include "Renderer.h"
#include <iostream>

class Texture
{
private:
	unsigned int  m_RendererID;
	std::string m_FilePath; //Í¼Æ¬Â·¾¶
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

	//ÊµÑé
	unsigned int ID;
	
	

public:
	Texture(const std::string& path);
	Texture();
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	unsigned int  GetTextureID() const ;




};
