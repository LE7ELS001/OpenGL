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
     shader.Bind(); //着色器是告诉GPU怎么处理数据
     ib.Bind();
     va.Bind();     //顶点数组是那对数据 （其中保活了vertexBuffer和indexBuffer)
     GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
 }

 void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const ShaderTwo& shaderTwo) const
 {
     shaderTwo.Bind(); //着色器是告诉GPU怎么处理数据
     ib.Bind();
     va.Bind();     //顶点数组是那对数据 （其中保活了vertexBuffer和indexBuffer)
     GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
 }


 void Renderer::Clear() const
 {
     GLCall(glClear(GL_COLOR_BUFFER_BIT));
 }
