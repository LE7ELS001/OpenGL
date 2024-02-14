#pragma once
#include "Renderer.h"
#include "Vendor/STB/stb_image.h"
#include <vector>
#include <iostream>

class CubeMap {
private:
	unsigned int m_RendererID;
	std::vector<std::string> faces;
	unsigned char *data;

	int height;
	int width;
	int nrChannels;

public:
	CubeMap(const std::string &face1, const std::string& face2, const std::string& face3, const std::string& face4, const std::string& face5, const std::string& face6); //暂时默认天空盒只有六个面
	~CubeMap();

	void loadCubeMap(std::vector<std::string> faces);

	void Bind(unsigned int slot = 0) const;
	void UnBind();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};
