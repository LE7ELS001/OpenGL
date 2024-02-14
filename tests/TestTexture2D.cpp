#include"TestTexture2D.h"




namespace test {
	TestTexture2D::TestTexture2D() : 
		m_translationA(200, 200, 0), m_translationB(400, 200, 0), m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		//三角形顶点
		float position[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f, 50.0f,  1.0f, 1.0f,
			-50.0f, 50.0f, 0.0f, 1.0f
		};

		//索引顺序
		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		//启用混合功能
		GLCall(glEnable(GL_BLEND));
		GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_Shader = std::make_unique<Shader>("src/res/Shaders/Texture2D.shader");
		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexbBuffer= std::make_unique<VertexBuffer>(position, 4 * 4 * sizeof(float)); //
		VertexBufferLayout layout;
		
		//写入layout数据
		layout.push<float>(2);
		layout.push<float>(2);


		//绑定着色器
		m_Shader->Bind();

		//调用vertexArray的addbuffer (直接帮你完成绑定VAO VBO, VertexArrayAttribPoint里的数据填写 启动顶点属性数组等）
		m_VertexArray->AddBuffer( *m_VertexbBuffer,  layout);

		//生成索引缓冲
		m_IndiceBuffer = std::make_unique<IndexBuffer>(indices, 6);

		//加载纹理
		m_Texture = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
		m_Shader->SetUniform1i("u_Texture", 0);

		//解绑VA和SHADER和IB
		m_VertexArray->UnBind();
		m_Shader->Unbind();
		m_IndiceBuffer->UnBind();
		m_VertexbBuffer->UnBind();
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer m_renderer;

		m_Texture->Bind();

		{
			//model
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 MVP = m_proj * m_view * model;
			m_Shader->Bind();
			//shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);
			m_Shader->SetUniformMat4f("u_MVP", MVP);
			//参数（模式：你要绘制什么; 顶点数组索引开始；顶点数量）
			m_renderer.Draw(*m_VertexArray, *m_IndiceBuffer, *m_Shader);
		}

		{
			//model
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			glm::mat4 MVP = m_proj * m_view * model;
			m_Shader->Bind();
			//shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);
			m_Shader->SetUniformMat4f("u_MVP", MVP);
			//参数（模式：你要绘制什么; 顶点数组索引开始；顶点数量）
			m_renderer.Draw(*m_VertexArray, *m_IndiceBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		{

			ImGui::SliderFloat3("translationA", &m_translationA.x, 0.0f, 960.0f);//&translation.x哪里会自动读取下面的           
			ImGui::SliderFloat3("translationB", &m_translationB.x, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
	}
}