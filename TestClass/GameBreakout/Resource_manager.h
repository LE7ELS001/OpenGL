#pragma once
#include "GameTexture.h"
#include "GameShader.h"

#include"GL/glew.h"

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Vendor/STB/stb_image.h"

class Resource_manager
{
public :
	static std::map<std::string, GameShader> Shaders;
	static std::map<std::string, GameTexture> Textures;

	//Shader相关
	static GameShader LoadShader(std::string path, std::string name);
	static GameShader GetShader(std::string name);

	//Texture相关
	static GameTexture LoadTexture(std::string path, bool alpha, std::string name);
	static GameTexture GetTexture(std::string name);

	static void Clear();

private:
	Resource_manager() {};

	static GameTexture loadTextureFromFile(std::string path, bool alpha);
};