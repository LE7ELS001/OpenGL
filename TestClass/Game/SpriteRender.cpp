#include "SpriteRender.h"

void SpriteRender::initRenderData()
{
	//VAO��VBO������
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

//������������draw ���ǳ�����ֱ�ӵ��� �����ĺ��������������������������� �����������
void SpriteRender::DrawSprite(const std::unique_ptr<Shader>& shader, const std::unique_ptr<Texture>& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	shader->Bind();
	model = glm::mat4(1.0f);

	//���������λ��
	model = glm::translate(model, glm::vec3(position, 0.0f));
	
	//λ�ƣ�Ϊ�˰���ת�����Ƶ��������ģ�
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	
	//��ת
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5 * size.y, 0.0));
	//����
	model = glm::scale(model, glm::vec3(size, 1.0f));

	//����ɫ��uniform
	shader->SetUniformMat4f("model", model);
	shader->SetUniformVec3("spriteColor", color);

	//������
	texture->Bind();

	//����
	m_VAO->Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	m_VAO->UnBind();
}

//�����ר�Ÿ�ש�������draw ���ڿ���������drawҲ�õ���
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
//	//���������λ��
//	model = glm::translate(model, glm::vec3(position, 0.0f));
//
//	//λ�ƣ�Ϊ�˰���ת�����Ƶ��������ģ�
//	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
//
//	//��ת
//	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
//
//	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5 * size.y, 0.0));
//	//����
//	model = glm::scale(model, glm::vec3(size, 1.0f));
//
//	//����ɫ��uniform
//	m_Shader->SetUniformMat4f("model", model);
//	m_Shader->SetUniformVec3("spriteColor", color);
//
//	//������
//	texture->Bind();
//
//	//����
//	m_VAO->Bind();
//	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
//	m_VAO->UnBind();
//}






