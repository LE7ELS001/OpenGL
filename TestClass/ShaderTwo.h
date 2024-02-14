#pragma once
#include<string>
#include<unordered_map>
#include "Vendor/glm/glm.hpp"


struct ShaderTwoProgramSource
{
	std::string VertexSource;
	std::string GeometrySource;
	std::string FragmentSource;
};

class ShaderTwo
{
private:
	unsigned int m_RendererID;
	std::string m_filePath;
	mutable std::unordered_map<std::string, int> m_Locationcache;

public:
	ShaderTwo(const std::string& filePath);
	~ShaderTwo();


	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int f1);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1iv(const std::string& name, int count, int* value);
	void SetUniformVec3(const std::string& name, const glm::vec3& value);
	void SetUniformVec3(const std::string& name, float x, float y, float z);
	void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	unsigned int m_ShaderTwoID;


private:
	unsigned int CreatShader(const std::string& vertexShaderTwo, const std::string& fragmentShaderTwo, std::string& geometryShaderTwo);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderTwoProgramSource ParseShader(const std::string& ShaderTwoFilePath);
	int GetUniformLocation(const std::string& name) const; //uniform name

};