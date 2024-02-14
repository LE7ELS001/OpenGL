#include "SpriteRender.h"

void SpriteRender::initRenderData()
{
	//VAO和VBO的设置
	VertexBufferLayout layout;
	layout.push<float>(4);
	m_VAO->AddBuffer(*m_VBO, layout);
	m_VAO->UnBind();
	
}

SpriteRender::SpriteRender()
{

	//=======================================
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	this->initRenderData();
}

SpriteRender::~SpriteRender()
{
}

//这个是最起初的draw 但是除非是直接调用 否则别的函数调用它会引发类似生存周期 作用域等问题
void SpriteRender::DrawSprite(const std::unique_ptr<Shader>& shader, const std::unique_ptr<Texture>& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	shader->Bind();
	model = glm::mat4(1.0f);

	//整个方格的位移
	model = glm::translate(model, glm::vec3(position, 0.0f));
	
	//位移（为了把旋转中心移到方格中心）
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	
	//旋转
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5 * size.y, 0.0));
	//缩放
	model = glm::scale(model, glm::vec3(size, 1.0f));

	//传着色器uniform
	shader->SetUniformMat4f("model", model);
	shader->SetUniformVec3("spriteColor", color);

	//绑定纹理
	texture->Bind();

	//绘制
	m_VAO->Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	m_VAO->UnBind();
}

//这个是专门给砖块那里的draw 后期可能其他的draw也用得上
void SpriteRender::DrawSpriteBrick(const std::unique_ptr<Shader>& shader,BrickTexture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	// prepare transformations
	shader->Bind();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

	shader->SetUniformMat4f("model", model);

	// render textured quad
	shader->SetUniformVec3("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	m_VAO->Bind();	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}



//void SpriteRender::DrawSprite(const std::unique_ptr<Texture>& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
//{
//	m_Shader->Bind();
//	model = glm::mat4(1.0f);
//
//	//整个方格的位移
//	model = glm::translate(model, glm::vec3(position, 0.0f));
//
//	//位移（为了把旋转中心移到方格中心）
//	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
//
//	//旋转
//	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
//
//	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5 * size.y, 0.0));
//	//缩放
//	model = glm::scale(model, glm::vec3(size, 1.0f));
//
//	//传着色器uniform
//	m_Shader->SetUniformMat4f("model", model);
//	m_Shader->SetUniformVec3("spriteColor", color);
//
//	//绑定纹理
//	texture->Bind();
//
//	//绘制
//	m_VAO->Bind();
//	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
//	m_VAO->UnBind();
//}






