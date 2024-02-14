#include "TestBatchRender.h"

#include "Renderer.h"
#include "Vendor/imgui/imgui.h"

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

namespace test {
	TestBatchRender::TestBatchRender() : m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float vertices[] = {
			100.0f, 100.0f, 0.0f,  0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 0.0f,  0.0f, //坐标 颜色 纹理坐标 纹理插槽
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

		//调用vertexArray的addbuffer (直接帮你完成绑定VAO VBO, VertexArrayAttribPoint里的数据填写 启动顶点属性数组等）
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//生成索引缓冲
		m_IndiceBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("src/res/Shaders/Basic.shader");

		//绑定着色器
		
		m_Shader->Bind();
		
		//加载纹理
		m_Texture1 = std::make_unique<Texture>("src/res/Picture/container.jpg");
		
		


		m_Texture2 = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
		
		
		m_Shader->SetUniform1i("u_Texture[0]", 0);
		m_Shader->SetUniform1i("u_Texture[1]", 1);




		//加载纹理
		//m_Texture = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
		//m_Texture->Bind();
		//m_Shader->SetUniform1i("u_Texture", 0);

		//解绑VA和SHADER和IB
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
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
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


