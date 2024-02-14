#include "ShaderTwo.h"

#include<iostream>;
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"


ShaderTwo::ShaderTwo(const std::string& filePath) : m_filePath(filePath), m_RendererID(0), m_ShaderTwoID(0)
{
    ShaderTwoProgramSource ShaderTwoSourceCode = ParseShader(filePath);//获取源代码（着色器x2的）
    m_RendererID = CreatShader(ShaderTwoSourceCode.VertexSource, ShaderTwoSourceCode.FragmentSource, ShaderTwoSourceCode.GeometrySource);//创建着色器
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
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));////参数 1.地址 2.有几个数组（这里是1），3.行列转换？ 4.数组地址
}






ShaderTwoProgramSource ShaderTwo::ParseShader(const std::string& ShaderTwoFilePath)
{
    std::ifstream stream(ShaderTwoFilePath);
    std::string line;
    std::stringstream ss[3]; //这里是两个字符串流 一个装vertex 一个装fragment 一个是默认的没有找到相关内容

    enum class ShaderTwotype//这个是决定那一段内容转成什么模式 从而对应地决定ss[]是1还是2
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };
    ShaderTwotype type = ShaderTwotype::NONE; //设置默认

    while (getline(stream, line)) //这个npos是指没有成功找到你填入的内容 这里反反得正 意味着找到了#ShaderTwo这个内容
    {
        if (line.find("#Shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)//这里是假如找到vertex 那么转入vertex模式
            {
                type = ShaderTwotype::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)//这里是假如找到fragment 那么转入fragment模式
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
            //这里会根据上面的模式 自动写入相应的模式
            ss[(int)type] << line << "\n";
        }
    }
    // 你需要返回两种不同的原始代码 但是正常情况下 你只能返回一种类型 解决方法是创造构造体 里面包含两种string 然后返回
    return { ss[0].str(), ss[2].str(),ss[1].str() };
}


////创建着色器（包括顶点和片段着色器）
unsigned int ShaderTwo::CreatShader(const std::string& vertexShaderTwo, const std::string& fragmentShaderTwo, std::string& geometryShaderTwo)
{
    unsigned int program = glCreateProgram();


    //顶点着色器编译
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderTwo);
    //片段着色器编译
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderTwo);
    //几何着色器
    unsigned int ge = CompileShader(GL_GEOMETRY_SHADER, geometryShaderTwo);


    //绑定着色器到程序
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glAttachShader(program, ge);

    //链接程序
    glLinkProgram(program);
    //启用程序
    glValidateProgram(program);

    //删除着色器 因为已经链接到程序了
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(ge);

    return program;
}




//编译着色器的函数
unsigned int ShaderTwo::CompileShader(unsigned int type, const std::string& source)
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

        std::cout << "Error, failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : (type == GL_GEOMETRY_SHADER ? "geometry" : "fragment")) << "the Infolog is   " << infolog
            << "the Infolog is   " << infolog << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

int ShaderTwo::GetUniformLocation(const std::string& name) const
{
    if (m_Locationcache.find(name) != m_Locationcache.end()) //查找缓存里有没有你的uniform 如果有直接返回 这样就不用反复访问GPU了
        return m_Locationcache[name];


    //假如没找到 就添加到cache里面
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));//获取ShaderTwo里面u_color位置的那个ID
    m_Locationcache[name] = location;
    return location;
}