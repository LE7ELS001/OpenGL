#include "Renderer.h"
#include "VertexArray.h"
#include"VertexBufferLayout.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements(); //unsigned int type;unsigned int count;unsigned char normalized;
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		if(elements[i].type == GL_FLOAT)  //这个是float的情况 
		{
			const auto& element = elements[i]; //对应里面的属性 比如第一个是位置坐标 第二个是纹理坐标
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		if (elements[i].type == GL_INT)  //这里是int的情况
		{
			const auto& element = elements[i]; //对应里面的属性 比如第一个是位置坐标 第二个是纹理坐标
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribIPointer(i, element.count, element.type,  layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		
	}

	

	

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}

unsigned int  VertexArray::GetId()
{
	return m_RendererID;
}
