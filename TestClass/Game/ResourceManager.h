#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Vendor/STB/stb_image.h"
#include "TestClass/Game/BrickTexture.h"

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


class ResourceManger
{
public:
	static std::map<std::string,  Shader> Shaders;
	static std::map<std::string, BrickTexture> Textures;

	

	//shader相关
	static Shader LoadShader(std::string name,std::string path);
	static Shader GetShader(std::string name);

	static ResourceManger& instance();

	//texture相关
	static BrickTexture LoadTexture(const std::string file, bool alpha, std::string name);
	static BrickTexture GetTexture(std::string name);

	static void Clear();

	static BrickTexture loadTextureFromFile(const std::string& path, bool alpha);

private:
		ResourceManger() {};

		
};
