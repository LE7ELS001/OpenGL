#include "GameShader.h"

void GameShader::GameShaderLoad(const std::string &path) 
{
    m_filePath = path;
    ShaderProgramSourceGame shaderSourceCode = ParseShader(path);//获取源代码（着色器x2的）
    m_ShaderID = CreatShader(shaderSourceCode.VertexSource, shaderSourceCode.FragmentSource);//创建着色器
    
}

ShaderProgramSourceGame GameShader::ParseShader(const std::string& shaderFilePath)
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

unsigned int GameShader::CreatShader(const std::string& vertexShader, const std::string& fragmentShader)
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

unsigned int GameShader::CompileShader(unsigned int type, const std::string& source)
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

GameShader& GameShader::Use()
{
    glUseProgram(this->m_ShaderID);
    return *this;
}

void GameShader::SetFloat(const char* name, float value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->m_ShaderID, name), value);
}

void GameShader::SetInterger(const char* name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->m_ShaderID, name), value);
}

void GameShader::SetVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->m_ShaderID, name), x,y);
}

void GameShader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->m_ShaderID, name), value.x, value.y);
}

void GameShader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->m_ShaderID, name), x,y,z);
}

void GameShader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->m_ShaderID, name), value.x, value.y, value.z);
}

void GameShader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->m_ShaderID, name), x,y,z,w);
}

void GameShader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->m_ShaderID, name), value.x, value.y, value.z, value.w);
}

void GameShader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->m_ShaderID, name), 1,false, glm::value_ptr(matrix));
}
