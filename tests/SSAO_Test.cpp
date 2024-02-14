#include "SSAO_Test.h"

namespace test {
	SSAO::SSAO(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(SSAO_YAW), Pitch(SSAO_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SSAO_SPEED), MouseSensitivity(SSAO_SENSITIVITY), Zoom(SSAO_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), LightPos(2.0f, 4.0f, -2.0f), blinn(false), blinnKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateSSAOVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小

			stbi_set_flip_vertically_on_load(true);
			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				 // bottom face
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 // top face
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};

			float quadVertices[] = {
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};




			//Shader
			m_ShaderGeometry = std::make_unique<Shader>("src/res/Shaders/SSAO/SSAO_Geometry.shader");
			m_LightShader = std::make_unique<Shader>("src/res/Shaders/SSAO/SSAO_Light.shader");
			m_SSAOshader = std::make_unique<Shader>("src/res/Shaders/SSAO/SSAO.shader");
			m_SSAOBlurShader = std::make_unique<Shader>("src/res/Shaders/SSAO/SSAO_Blur.shader");

			m_LightShader->Bind();
			m_LightShader->SetUniform1i("gPosition", 0);
			m_LightShader->SetUniform1i("gNormal", 1);
			m_LightShader->SetUniform1i("gAlbedo", 2);
			m_LightShader->SetUniform1i("ssao", 3);

			m_SSAOshader->Bind();
			m_SSAOshader->SetUniform1i("gPosition", 0);
			m_SSAOshader->SetUniform1i("gNormal", 1);
			m_SSAOshader->SetUniform1i("texNoise", 2);

			m_SSAOBlurShader->Bind();
			m_SSAOBlurShader->SetUniform1i("ssaoInput", 0);

			//VAB VBO
			m_VertexArray = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(&vertices, sizeof(vertices));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArray->AddBuffer(*m_VertexbBuffer, layout);

			//Quad_VAO,VBO
			m_VertexArrayQuad = std::make_unique<VertexArray>();
			m_VertexBufferQuad = std::make_unique<VertexBuffer>(&quadVertices, sizeof(quadVertices));
			VertexBufferLayout QuadLayout;
			QuadLayout.push<float>(3);
			QuadLayout.push<float>(2);
			m_VertexArrayQuad->AddBuffer(*m_VertexBufferQuad, QuadLayout);


			//model
			
			std::string path = "src/res/Model/nanosuit/nanosuit.obj";
			m_backpack = std::make_unique<Model>(path, false);

			

			//g_buffer
			m_GBUffer = std::make_unique<G_buffer>();

			//SSAO_FBO
			m_SSAO = std::make_unique<FBO_SSAO>();

			//SSAO_Blur
			m_SSAO_Blur = std::make_unique<FBO_SSAO_Blur>();


			//采样半球
			std::uniform_real_distribution<GLfloat> randomFloat(0.0, 1.0);
			std::default_random_engine generator;
			
			for (unsigned int  i = 0; i < 64; i++)
			{
				//范围调整成[-1,1]
				glm::vec3 sample(randomFloat(generator) * 2.0 - 1.0, randomFloat(generator) * 2.0 - 1.0, randomFloat(generator)); 
				//在范围内标准化
				sample = glm::normalize(sample);
				//标准化后再随机长短（保证随机结果必然再标准化范围内）
				sample *= randomFloat(generator);

				float scale = float(i) / 64.0f;

				scale = ourLerp(0.1f, 1.0f, scale * scale);
				sample *= scale;
				SSAO_kernel.push_back(sample);
			}

			//随机旋转采样（没太懂）
			for (unsigned int  i = 0; i < 16; i++)
			{
				glm::vec3 noise(randomFloat(generator) * 2.0 - 1.0, randomFloat(generator) * 2.0 - 1.0, 0.0f);
				SSAO_noise.push_back(noise);
			}

			//NoiseTexture
			m_NoiseTexutre = std::make_unique<NoiseTexture>(SSAO_noise);
			
		}


	}


	SSAO::SSAO(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateSSAOVectors();
	}



	SSAO::~SSAO()
	{
	}

	glm::mat4 SSAO::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void SSAO::ProcessKeyboard(My_SSAO_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == SSAO_FORWARD)
			Position += Front * velocity;
		if (direction == SSAO_BACKWARD)
			Position -= Front * velocity;
		if (direction == SSAO_LEFT)
			Position -= Right * velocity;
		if (direction == SSAO_RIGHT)
			Position += Right * velocity;
	}

	void SSAO::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
	{
		glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && CursorEnable == false)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			setCursorMode(window, GLFW_CURSOR_DISABLED);
			CursorEnable = true;

		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			this->ProcessKeyboard(SSAO_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(SSAO_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(SSAO_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(SSAO_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKey)
		{
			blinn = !blinn;
			blinnKey = true;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			blinnKey = false;
		}
	}

	void SSAO::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		updateSSAOVectors();
	}

	void SSAO::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void SSAO::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void SSAO::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		SSAO* instance = static_cast<SSAO*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void SSAO::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void SSAO::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (FirstMouse)
		{
			this->lastX = static_cast<float>(xpos);
			this->lastY = static_cast<float>(ypos);
			FirstMouse = false;
		}
		float xoffset = xpos - this->lastX;
		float yoffset = this->lastY - ypos;

		this->lastX = static_cast<float>(xpos);
		this->lastY = static_cast<float>(ypos);
		this->ProcessMouseMovement(xoffset, yoffset);
	}

	void SSAO::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		SSAO* instance = static_cast<SSAO*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void SSAO::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	float SSAO::ourLerp(float a, float b, float f)
	{
		return a + f * (b - a );
	}

	void SSAO::OnUpdate(float deltaTime)
	{

	}

	void SSAO::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色


		//g_buffer获取所需图像
		m_GBUffer->Bind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 50.0f);
		m_model = glm::mat4(1.0f);

		m_ShaderGeometry->Bind();
		m_ShaderGeometry->SetUniformMat4f("projection", m_proj);
		m_ShaderGeometry->SetUniformMat4f("view", m_view);

		//环境箱子属性设定
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0f, 7.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(10.5f, 7.5f, 17.5f));
		m_ShaderGeometry->SetUniformMat4f("model", m_model);
		m_ShaderGeometry->SetUniform1i("invertedNormals", true);
		//绘制
		m_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		m_VertexArray->UnBind();
		m_ShaderGeometry->SetUniform1i("invertedNormals", false);

		//模型位置
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0f, 0.5f, 0.0f));
		m_model = glm::rotate(m_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(1.0f));
		m_ShaderGeometry->SetUniformMat4f("model", m_model);
		m_backpack->Draw(*m_ShaderGeometry.get());

		m_GBUffer->UnBind();

		//SSAO处理阶段
		m_SSAO->Bind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		m_SSAOshader->Bind();
		for (unsigned int  i = 0; i < 64; i++)
		{
			m_SSAOshader->SetUniformVec3("samples[" + std::to_string(i) + "]", SSAO_kernel[i]);
		}
		m_SSAOshader->SetUniformMat4f("projection", m_proj);
		m_GBUffer->BindPosition(0);
		m_GBUffer->BindNormal(1);
		m_NoiseTexutre->BindNoiseTexture(2);

		//绘制效果
		m_VertexArrayQuad->Bind();
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		m_VertexArrayQuad->UnBind();
		m_SSAO->UnBind();


		//模糊效果
		m_SSAO_Blur->Bind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		m_SSAOBlurShader->Bind();
		m_SSAO->BindSSAO_RT(0);
		//绘制效果
		m_VertexArrayQuad->Bind();
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		m_VertexArrayQuad->UnBind();
		m_SSAO_Blur->UnBind();


		//光照处理
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		m_LightShader->Bind();
		m_lightPosview = glm::vec3(GetViewMatrix() * glm::vec4(LightPos,1.0));
		m_LightShader->SetUniformVec3("light.Position", m_lightPosview);
		m_LightShader->SetUniformVec3("light.Color", lightColor);

		m_LightShader->SetUniform1f("light.Linear", linear);
		m_LightShader->SetUniform1f("light.Quadratic", quadratic);

		m_GBUffer->BindPosition(0);
		m_GBUffer->BindNormal(1);
		m_GBUffer->BindAlbedo(2);
		m_SSAO_Blur->BindBlurRt(3);
		m_VertexArrayQuad->Bind();
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		m_VertexArrayQuad->UnBind();

	}

	void SSAO::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void SSAO::updateSSAOVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void SSAO::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& SSAO::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}