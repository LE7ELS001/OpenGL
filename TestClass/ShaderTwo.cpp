#include "ShaderTwo.h"

#include<iostream>;
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"


ShaderTwo::ShaderTwo(const std::string& filePath) : m_filePath(filePath), m_RendererID(0), m_ShaderTwoID(0)
{
    ShaderTwoProgramSource ShaderTwoSourceCode = ParseShader(filePath);//��ȡԴ���루��ɫ��x2�ģ�
    m_RendererID = CreatShader(ShaderTwoSourceCode.VertexSource, ShaderTwoSourceCode.FragmentSource, ShaderTwoSourceCode.GeometrySource);//������ɫ��
    m_ShaderTwoID = m_RendererID;
}


ShaderTwo::~ShaderTwo()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void ShaderTwo::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void ShaderTwo::Unbind() const
{
    GLCall(glUseProgram(0));
}

void ShaderTwo::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, f1, f2, f3, f4));
}

void ShaderTwo::SetUniform1i(const std::string& name, int f1)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, f1));
}

void ShaderTwo::SetUniform1f(const std::string& name, float value)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value));
}

void ShaderTwo::SetUniform1iv(const std::string& name, int count, int* value)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform1iv(location, count, value));
}

void ShaderTwo::SetUniformVec3(const std::string& name, const glm::vec3& value)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform3fv(location, 1, &value[0]));
}

void ShaderTwo::SetUniformVec3(const std::string& name, float x, float y, float z)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform3f(location, x, y, z));
}


void ShaderTwo::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));////���� 1.��ַ 2.�м������飨������1����3.����ת���� 4.�����ַ
}






ShaderTwoProgramSource ShaderTwo::ParseShader(const std::string& ShaderTwoFilePath)
{
    std::ifstream stream(ShaderTwoFilePath);
    std::string line;
    std::stringstream ss[3]; //�����������ַ����� һ��װvertex һ��װfragment һ����Ĭ�ϵ�û���ҵ��������

    enum class ShaderTwotype//����Ǿ�����һ������ת��ʲôģʽ �Ӷ���Ӧ�ؾ���ss[]��1����2
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };
    ShaderTwotype type = ShaderTwotype::NONE; //����Ĭ��

    while (getline(stream, line)) //���npos��ָû�гɹ��ҵ������������ ���ﷴ������ ��ζ���ҵ���#ShaderTwo�������
    {
        if (line.find("#Shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)//�����Ǽ����ҵ�vertex ��ôת��vertexģʽ
            {
                type = ShaderTwotype::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)//�����Ǽ����ҵ�fragment ��ôת��fragmentģʽ
            {
                type = ShaderTwotype::FRAGMENT;
            }
            else if (line.find("geometry") != std::string::npos)
            {
                type = ShaderTwotype::GEOMETRY;
            }
        }
        else
        {
            //�������������ģʽ �Զ�д����Ӧ��ģʽ
            ss[(int)type] << line << "\n";
        }
    }
    // ����Ҫ�������ֲ�ͬ��ԭʼ���� ������������� ��ֻ�ܷ���һ������ ��������Ǵ��칹���� �����������string Ȼ�󷵻�
    return { ss[0].str(), ss[2].str(),ss[1].str() };
}


////������ɫ�������������Ƭ����ɫ����
unsigned int ShaderTwo::CreatShader(const std::string& vertexShaderTwo, const std::string& fragmentShaderTwo, std::string& geometryShaderTwo)
{
    unsigned int program = glCreateProgram();


    //������ɫ������
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderTwo);
    //Ƭ����ɫ������
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderTwo);
    //������ɫ��
    unsigned int ge = CompileShader(GL_GEOMETRY_SHADER, geometryShaderTwo);


    //����ɫ��������
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glAttachShader(program, ge);

    //���ӳ���
    glLinkProgram(program);
    //���ó���
    glValidateProgram(program);

    //ɾ����ɫ�� ��Ϊ�Ѿ����ӵ�������
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(ge);

    return program;
}




//������ɫ���ĺ���
unsigned int ShaderTwo::CompileShader(unsigned int type, const std::string& source)
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

        std::cout << "Error, failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : (type == GL_GEOMETRY_SHADER ? "geometry" : "fragment")) << "the Infolog is   " << infolog
            << "the Infolog is   " << infolog << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

int ShaderTwo::GetUniformLocation(const std::string& name) const
{
    if (m_Locationcache.find(name) != m_Locationcache.end()) //���һ�������û�����uniform �����ֱ�ӷ��� �����Ͳ��÷�������GPU��
        return m_Locationcache[name];


    //����û�ҵ� ����ӵ�cache����
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));//��ȡShaderTwo����u_colorλ�õ��Ǹ�ID
    m_Locationcache[name] = location;
    return location;
}