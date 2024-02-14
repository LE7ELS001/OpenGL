#include"Shader.h"

#include<iostream>;
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"








Shader::Shader(const std::string& filePath) : m_filePath(filePath), m_RendererID(0), m_shaderID(0)
{
    ShaderProgramSource shaderSourceCode = ParseShader(filePath);//获取源代码（着色器x2的）
    m_RendererID = CreatShader(shaderSourceCode.VertexSource, shaderSourceCode.FragmentSource);//创建着色器
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
    GLCall(glUniformMatrix4fv(location, 1 ,GL_FALSE, &matrix[0][0]));////参数 1.地址 2.有几个数组（这里是1），3.行列转换？ 4.数组地址
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
    std::stringstream ss[2]; //这里是两个字符串流 一个装vertex 一个装fragment 一个是默认的没有找到相关内容

    enum class Shadertype//这个是决定那一段内容转成什么模式 从而对应地决定ss[]是1还是2
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    Shadertype type = Shadertype::NONE; //设置默认

    while (getline(stream, line)) //这个npos是指没有成功找到你填入的内容 这里反反得正 意味着找到了#shader这个内容
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)//这里是假如找到vertex 那么转入vertex模式
            {
                type = Shadertype::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)//这里是假如找到fragment 那么转入fragment模式
            {
                type = Shadertype::FRAGMENT;
            }
        }
        else
        {
            //这里会根据上面的模式 自动写入相应的模式
            ss[(int)type] << line << "\n";
        }
    }
    // 你需要返回两种不同的原始代码 但是正常情况下 你只能返回一种类型 解决方法是创造构造体 里面包含两种string 然后返回
    return { ss[0].str(), ss[1].str() };
}


////创建着色器（包括顶点和片段着色器）
unsigned int Shader::CreatShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();


    //顶点着色器编译
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    //片段着色器编译
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


    //绑定着色器到程序
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //链接程序
    glLinkProgram(program);
    //启用程序
    glValidateProgram(program);

    //删除着色器 因为已经链接到程序了
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




//编译着色器的函数
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //着色器源码
    glShaderSource(id, 1, &src, nullptr); //着色器ID , 源码数量 ， 源码内容
    glCompileShader(id);

    //检查是否链接着色器出问题了 出问题时返回警告 以及报告
    int success;//是否成功
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);//检查那个着色器， 检查什么状态， 返回值传给谁
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
    if (m_Locationcache.find(name) != m_Locationcache.end()) //查找缓存里有没有你的uniform 如果有直接返回 这样就不用反复访问GPU了
        return m_Locationcache[name];


    //假如没找到 就添加到cache里面
    GLCall(int location = glGetUniformLocation(m_RendererID,name.c_str()));//获取shader里面u_color位置的那个ID
    m_Locationcache[name] = location;
    return location;
}

