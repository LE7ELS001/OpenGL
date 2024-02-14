#include "ResourceManager.h"


std::map<std::string,  Shader> ResourceManger::Shaders;
std::map<std::string,  BrickTexture> ResourceManger::Textures;


Shader ResourceManger::LoadShader(std::string name, std::string path)
{
	Shader tmp = Shader(path);
	Shaders[name] = tmp;
	return Shaders[name];
}

Shader ResourceManger::GetShader(std::string name)
{
	return Shaders[name];
}

ResourceManger& ResourceManger::instance()
{
		static ResourceManger instance;
		return instance;
}

BrickTexture ResourceManger::LoadTexture(const std::string file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

BrickTexture ResourceManger::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManger::Clear() //É¾³ýshaderºÍTexture
{
	for (auto iter : Shaders)
	{
		GLCall(glDeleteProgram(iter.second.m_shaderID))
	}

	for (auto iter : Textures)
	{
		unsigned int Tmp_ID = iter.second.ID;
		GLCall(glDeleteTextures(1, &Tmp_ID));
	}
}

BrickTexture ResourceManger::loadTextureFromFile(const std::string& path, bool alpha)
{
	// create texture object
	BrickTexture texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	// now generate texture
	texture.Generate(width, height, data);
	// and finally free image data
	stbi_image_free(data);
	return texture;
}



