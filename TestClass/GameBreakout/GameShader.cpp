#include "GameShader.h"

void GameShader::GameShaderLoad(const std::string &path) 
{
    m_filePath = path;
    ShaderProgramSourceGame shaderSourceCode = ParseShader(path);//��ȡԴ���루��ɫ��x2�ģ�
    m_ShaderID = CreatShader(shaderSourceCode.VertexSource, shaderSourceCode.FragmentSource);//������ɫ��
    
}

ShaderProgramSourceGame GameShader::ParseShader(const std::string& shaderFilePath)
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

unsigned int GameShader::CreatShader(const std::string& vertexShader, const std::string& fragmentShader)
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

unsigned int GameShader::CompileShader(unsigned int type, const std::string& source)
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
