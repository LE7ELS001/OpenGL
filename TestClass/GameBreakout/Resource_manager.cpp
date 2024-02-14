#include "Resource_manager.h"

//实例化静态变量
std::map<std::string, GameShader> Resource_manager::Shaders;
std::map<std::string, GameTexture> Resource_manager::Textures;

GameShader Resource_manager::LoadShader(std::string path, std::string name)
{
	GameShader tmp_Shader;
	tmp_Shader.GameShaderLoad(path);
	Shaders[name] = tmp_Shader;
	return Shaders[name];
}

GameShader Resource_manager::GetShader(std::string name)
{
	return Shaders[name];
}

GameTexture Resource_manager::LoadTexture(std::string path, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(path, alpha);
	return Textures[name];
}	

GameTexture Resource_manager::GetTexture(std::string name)
{
	return Textures[name];
}

void Resource_manager::Clear()
{
	for (auto iter : Shaders)
	{
		glDeleteShader(iter.second.m_ShaderID);
	}
	for (auto iter : Textures)
	{
		glDeleteTextures(1, &iter.second.TextureID);
	}
}

GameTexture Resource_manager::loadTextureFromFile(std::string path, bool alpha)
{
	GameTexture tmp_Texture;
	if (alpha)
	{
		tmp_Texture.Internal_format = GL_RGBA;
		tmp_Texture.Image_format = GL_RGBA;
	}
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	tmp_Texture.Generate(width, height, data);

	stbi_image_free(data);
	return tmp_Texture;
}
