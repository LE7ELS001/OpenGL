#include"Shader.h"

#include<iostream>;
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"








Shader::Shader(const std::string& filePath) : m_filePath(filePath), m_RendererID(0), m_shaderID(0)
{
    ShaderProgramSource shaderSourceCode = ParseShader(filePath);//��ȡԴ���루��ɫ��x2�ģ�
    m_RendererID = CreatShader(shaderSourceCode.VertexSource, shaderSourceCode.FragmentSource);//������ɫ��
    m_shaderID = m_RendererID;
}

Shader::Shader() : m_RendererID(0), m_shaderID(0)
{
}


Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location , f1, f2, f3, f4));
}

void Shader::SetUniform1i(const std::string& name, int f1) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, f1));
}

void Shader::SetUniform1f(const std::string& name, float value) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value));
}

void Shader::SetUniform1iv(const std::string& name, int count, int* value) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1iv(location, count, value));
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform3fv(location, 1, &value[0]));
}

void Shader::SetUniformVec3(const std::string& name, float x, float y, float z) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform3f(location, x, y, z));
}

void Shader::SetUniform3VecFloat(const std::string& name, int count, const float* value) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniform3fv(location, count, value));
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1 ,GL_FALSE, &matrix[0][0]));////���� 1.��ַ 2.�м������飨������1����3.����ת���� 4.�����ַ
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3 matrix) const
{
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]));
}






ShaderProgramSource Shader::ParseShader(const std::string& shaderFilePath)
{
    std::ifstream stream(shaderFilePath);
    std::string line;
    std::stringstream ss[2]; //�����������ַ����� һ��װvertex һ��װfragment һ����Ĭ�ϵ�û���ҵ��������

    enum class Shadertype//����Ǿ�����һ������ת��ʲôģʽ �Ӷ���Ӧ�ؾ���ss[]��1����2
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    Shadertype type = Shadertype::NONE; //����Ĭ��

    while (getline(stream, line)) //���npos��ָû�гɹ��ҵ������������ ���ﷴ������ ��ζ���ҵ���#shader�������
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)//�����Ǽ����ҵ�vertex ��ôת��vertexģʽ
            {
                type = Shadertype::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)//�����Ǽ����ҵ�fragment ��ôת��fragmentģʽ
            {
                type = Shadertype::FRAGMENT;
            }
        }
        else
        {
            //�������������ģʽ �Զ�д����Ӧ��ģʽ
            ss[(int)type] << line << "\n";
        }
    }
    // ����Ҫ�������ֲ�ͬ��ԭʼ���� ������������� ��ֻ�ܷ���һ������ ��������Ǵ��칹���� �����������string Ȼ�󷵻�
    return { ss[0].str(), ss[1].str() };
}


////������ɫ�������������Ƭ����ɫ����
unsigned int Shader::CreatShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();


    //������ɫ������
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    //Ƭ����ɫ������
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


    //����ɫ��������
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //���ӳ���
    glLinkProgram(program);
    //���ó���
    glValidateProgram(program);

    //ɾ����ɫ�� ��Ϊ�Ѿ����ӵ�������
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




//������ɫ���ĺ���
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //��ɫ��Դ��
    glShaderSource(id, 1, &src, nullptr); //��ɫ��ID , Դ������ �� Դ������
    glCompileShader(id);

    //����Ƿ�������ɫ���������� ������ʱ���ؾ��� �Լ�����
    int success;//�Ƿ�ɹ�
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);//����Ǹ���ɫ���� ���ʲô״̬�� ����ֵ����˭
    if (!success)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* infolog = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, infolog);

        std::cout << "Error, failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "the Infolog is   " << infolog << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_Locationcache.find(name) != m_Locationcache.end()) //���һ�������û�����uniform �����ֱ�ӷ��� �����Ͳ��÷�������GPU��
        return m_Locationcache[name];


    //����û�ҵ� ����ӵ�cache����
    GLCall(int location = glGetUniformLocation(m_RendererID,name.c_str()));//��ȡshader����u_colorλ�õ��Ǹ�ID
    m_Locationcache[name] = location;
    return location;
}

