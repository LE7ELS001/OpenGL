#include"Renderer.h"

#include<iostream>

 void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
};

 bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Opengl Error] (" << error << "): " << function << " " << file << " : " << line << std::endl;
        return false;
    }

    return true;
}

 void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
 {
     shader.Bind(); //��ɫ���Ǹ���GPU��ô��������
     ib.Bind();
     va.Bind();     //�����������Ƕ����� �����б�����vertexBuffer��indexBuffer)
     GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
 }

 void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const ShaderTwo& shaderTwo) const
 {
     shaderTwo.Bind(); //��ɫ���Ǹ���GPU��ô��������
     ib.Bind();
     va.Bind();     //�����������Ƕ����� �����б�����vertexBuffer��indexBuffer)
     GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
 }


 void Renderer::Clear() const
 {
     GLCall(glClear(GL_COLOR_BUFFER_BIT));
 }