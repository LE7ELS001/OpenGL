#pragma once
#include <string>
#include<iostream>;
#include<fstream>
#include<string>
#include<sstream>

#include <GL/glew.h>
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"

struct ShaderProgramSourceGame
{
	std::string VertexSource;
	std::string FragmentSource;
};

class GameShader
{
public:
	unsigned int m_ShaderID;
	std::string m_filePath;

	GameShader()  : m_ShaderID(0), m_filePath("path") {};

	void GameShaderLoad(const std::string &path);

	ShaderProgramSourceGame ParseShader(const std::string& shaderFilePath);
	unsigned int CreatShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);

	GameShader& Use();

	void SetFloat(const char* name, float value, bool useShader = false);
	void SetInterger(const char* name, int value, bool useShader = false);
	void SetVector2f(const char* name, float x, float y, bool useShader = false);
	void SetVector2f(const char* name, const glm::vec2 & value, bool useShader = false);
	void SetVector3f(const char* name, float x, float y,  float z, bool useShader = false);
	void SetVector3f(const char* name, const glm::vec3 & value, bool useShader = false);
	void SetVector4f(const char* name, float x, float y, float z,float w, bool useShader = false);
	void SetVector4f(const char* name, const glm::vec4 &value, bool useShader = false);
	void SetMatrix4(const char* name, const glm::mat4 &matrix, bool useShader = false);
};
