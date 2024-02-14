#include "TestBatchRender.h"

#include "Renderer.h"
#include "Vendor/imgui/imgui.h"

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

namespace test {
	TestBatchRender::TestBatchRender() : m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float vertices[] = {
			100.0f, 100.0f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 0.0f,  0.0f, //���� ��ɫ �������� ������
			200.0f, 100.0f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 0.0f,  0.0f,
			200.0f, 200.0f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 1.0f,  0.0f,
			100.0f, 200.0f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 1.0f,  0.0f,

			300.0f, 100.0f, 0.0f,   1.0f,0.93f ,0.24f, 1.0f,   0.0f, 0.0f,   1.0f,
			400.0f, 100.0f, 0.0f,   1.0f,0.93f ,0.24f, 1.0f,   1.0f, 0.0f,   1.0f,
			400.0f, 200.0f, 0.0f,   1.0f,0.93f ,0.24f, 1.0f,   1.0f, 1.0f,   1.0f,
			300.0f, 200.0f, 0.0f,   1.0f,0.93f ,0.24f, 1.0f,   0.0f, 1.0f,   1.0f

		};

		unsigned int indices[] = {
			0,1,2,2,3,0,
			4,5,6,6,7,4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 10 * sizeof(float)); //
		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(4);
		layout.push<float>(2);
		layout.push<float>(1);

		//����vertexArray��addbuffer (ֱ�Ӱ�����ɰ�VAO VBO, VertexArrayAttribPoint���������д ����������������ȣ�
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//������������
		m_IndiceBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("src/res/Shaders/Basic.shader");

		//����ɫ��
		
		m_Shader->Bind();
		
		//��������
		m_Texture1 = std::make_unique<Texture>("src/res/Picture/container.jpg");
		
		


		m_Texture2 = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
		
		
		m_Shader->SetUniform1i("u_Texture[0]", 0);
		m_Shader->SetUniform1i("u_Texture[1]", 1);




		//��������
		//m_Texture = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
		//m_Texture->Bind();
		//m_Shader->SetUniform1i("u_Texture", 0);

		//���VA��SHADER��IB
		m_VAO->UnBind();
		m_Shader->Unbind();
		m_IndiceBuffer->UnBind();
		m_VertexBuffer->UnBind();
	}

	TestBatchRender::~TestBatchRender()
	{
	}

	void TestBatchRender::OnUpdate(float deltaTime)
	{
		
	}

	void TestBatchRender::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //����������ɫ������ ���趨һ����ɫ
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		glm::mat4 mvp = m_proj * m_view;

		m_Texture1->Bind();
		m_Texture2->Bind(1);

		
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("m_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndiceBuffer, *m_Shader);



	}

	void TestBatchRender::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}


