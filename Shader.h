#pragma once
#include<string>
#include<unordered_map>
#include "Vendor/glm/glm.hpp"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_filePath;
	mutable std::unordered_map<std::string, int> m_Locationcache;

public:
	Shader(const std::string& filePath);
	Shader();
	~Shader();


	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int f1) const;
	void SetUniform1f(const std::string& name, float value) const;
	void SetUniform1iv(const std::string& name, int count, int* value) const;
	void SetUniformVec3(const std::string& name, const glm::vec3& value) const;
	void SetUniformVec3(const std::string& name, float x, float y, float z) const;
	void SetUniform3VecFloat(const std::string& name, int count, const float* value) const;
	void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4) const;
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;
	void SetUniformMat3(const std::string& name, const glm::mat3 matrix) const;

	unsigned int m_shaderID;


private:
	unsigned int CreatShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& shaderFilePath);
	int GetUniformLocation(const std::string& name) const; //uniform name

};