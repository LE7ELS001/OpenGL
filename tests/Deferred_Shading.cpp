#include "Deferred_Shading.h"

namespace test {
	DeferredShading::DeferredShading(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(DES_YAW), Pitch(DES_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(DES_SPEED), MouseSensitivity(DES_SENSITIVITY), Zoom(DES_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), LightPos(0.0f, 0.0f, 0.0f), blinn(false), blinnKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateDeferredShadingVectors();


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
			m_Shader = std::make_unique<Shader>("src/res/Shaders/Deferred Shading/G_Buffer.shader");
			m_LightHandler = std::make_unique<Shader>("src/res/Shaders/Deferred Shading/DeferredLight.shader");
			m_LightBox = std::make_unique<Shader>("src/res/Shaders/Deferred Shading/DeferredLightBox.shader");

			m_LightHandler->Bind();
			m_LightHandler->SetUniform1i("gPosition", 0);
			m_LightHandler->SetUniform1i("gNormal", 1);
			m_LightHandler->SetUniform1i("gAlbedoSpec", 2);

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
			//std::string path = "src/res/Model/rock/rock.obj";
			//std::string path = "src/res/Model/backpack/backpack.obj";
			std::string path = "src/res/Model/nanosuit/nanosuit.obj";
			m_backpack = std::make_unique<Model>(path, false);

			//模型位置
			objectPosition.push_back(glm::vec3(-3.0, -0.5, -3.0));
			objectPosition.push_back(glm::vec3(0.0, -0.5, -3.0));
			objectPosition.push_back(glm::vec3(3.0, -0.5, -3.0));
			objectPosition.push_back(glm::vec3(-3.0, -0.5, 0.0));
			objectPosition.push_back(glm::vec3(0.0, -0.5, 0.0));
			objectPosition.push_back(glm::vec3(3.0, -0.5, 0.0));
			objectPosition.push_back(glm::vec3(-3.0, -0.5, 3.0));
			objectPosition.push_back(glm::vec3(0.0, -0.5, 3.0));
			objectPosition.push_back(glm::vec3(3.0, -0.5, 3.0));

			//g_buffer
			m_GBUffer = std::make_unique<G_buffer>();


			//光源位置
			srand(13);
			for (unsigned int i = 0; i < NR_LIGHT; i++)
			{
				float xpos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
				float ypos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
				float zpos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
				LightPosition.push_back(glm::vec3(xpos, ypos, zpos));

				float r = static_cast<float>(((rand() % 100) / 200.0) + 0.5);
				float g = static_cast<float>(((rand() % 100) / 200.0) + 0.5);
				float b = static_cast<float>(((rand() % 100) / 200.0) + 0.5);
				LightColor.push_back(glm::vec3(r, g, b));
			}
			

			//测试
			amount = objectPosition.size();
			matrices = new glm::mat4[amount];
			for (unsigned int i = 0; i < amount; i++)
			{
				m_model = glm::mat4(1.0f);
				m_model = glm::translate(m_model, objectPosition[i]);
				m_model = glm::scale(m_model, glm::vec3(0.5f));
				matrices[i] = m_model;
			}

			//m_InstanceVAO = std::make_unique<VertexArray>();
			m_InstanceVBO = std::make_unique<VertexBuffer>(&matrices[0], amount * sizeof(glm::mat4));

			for (unsigned int i = 0; i < m_backpack->meshes.size(); i++)
			{
				unsigned int VAO = m_backpack->meshes[i].VAO;
				glBindVertexArray(VAO);
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

				glVertexAttribDivisor(3, 1);
				glVertexAttribDivisor(4, 1);
				glVertexAttribDivisor(5, 1);
				glVertexAttribDivisor(6, 1);

				glBindVertexArray(0);
			}

			for (unsigned int  i = 0; i < m_backpack->textures_loaded.size(); i++)
			{
				std::cout << m_backpack->textures_loaded[i].type << "\n" << std::endl;
			}
		}


	}


	DeferredShading::DeferredShading(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateDeferredShadingVectors();
	}



	DeferredShading::~DeferredShading()
	{
	}

	glm::mat4 DeferredShading::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void DeferredShading::ProcessKeyboard(My_DeferredShading_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == DES_FORWARD)
			Position += Front * velocity;
		if (direction == DES_BACKWARD)
			Position -= Front * velocity;
		if (direction == DES_LEFT)
			Position -= Right * velocity;
		if (direction == DES_RIGHT)
			Position += Right * velocity;
	}

	void DeferredShading::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(DES_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(DES_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(DES_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(DES_RIGHT, deltaTime);

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

	void DeferredShading::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateDeferredShadingVectors();
	}

	void DeferredShading::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void DeferredShading::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void DeferredShading::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		DeferredShading* instance = static_cast<DeferredShading*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void DeferredShading::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void DeferredShading::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void DeferredShading::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		DeferredShading* instance = static_cast<DeferredShading*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void DeferredShading::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void DeferredShading::OnUpdate(float deltaTime)
	{

	}

	void DeferredShading::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		m_GBUffer->Bind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		m_model = glm::mat4(1.0f);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);
		
		//g缓冲获取4种图像
		//for (unsigned int i = 0; i < objectPosition.size(); i++)
		//{
		//	m_model = glm::mat4(1.0f);
		//	m_model = glm::translate(m_model, objectPosition[i]);
		//	m_model = glm::scale(m_model, glm::vec3(0.5f));
		//	m_Shader->SetUniformMat4f("model", m_model);
		//	m_backpack->Draw(*m_Shader.get());
		//}
		m_Shader->SetUniform1i("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_backpack->textures_loaded[0].ID);
		m_Shader->SetUniform1i("texture_specular1", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_backpack->textures_loaded[4].ID);
		for (unsigned int i = 0; i < m_backpack->meshes.size(); i++)
		{
			glBindVertexArray(m_backpack->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(m_backpack->meshes[i].Me_indices.size()), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

		//默认帧缓冲处理光照信息
		m_GBUffer->UnBind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		m_LightHandler->Bind();

		m_GBUffer->BindPosition(0);
		m_GBUffer->BindNormal(1);
		m_GBUffer->BindAlbedo(2);

		for (unsigned int i = 0; i < LightPosition.size(); i++)
		{
			m_LightHandler->SetUniformVec3("lights[" + std::to_string(i) + "].Position", LightPosition[i]);
			m_LightHandler->SetUniformVec3("lights[" + std::to_string(i) + "].Color", LightColor[i]);

			m_LightHandler->SetUniform1f("lights[" + std::to_string(i) + "].Linear", linear);
			m_LightHandler->SetUniform1f("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		}
		m_LightHandler->SetUniformVec3("viewPos", Position);
		
		m_VertexArrayQuad->Bind();
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		m_VertexArrayQuad->UnBind();

		//绘制后传输深度信息到默认的帧缓冲深度信息rbo中
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBUffer->GetBufferID()));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));

		GLCall(glBlitFramebuffer(0, 0, 960.0f, 540.0f, 0, 0, 960.0f, 540.0f, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
		m_GBUffer->UnBind();

		//绘制光源
		m_LightBox->Bind();
		m_LightBox->SetUniformMat4f("projection", m_proj);
		m_LightBox->SetUniformMat4f("view", m_view);
		
		for (unsigned int  i = 0; i < LightPosition.size(); i++)
		{
			m_model = glm::mat4(1.0f);
			m_model = glm::translate(m_model, LightPosition[i]);
			m_model = glm::scale(m_model, glm::vec3(0.125f));
			m_LightBox->SetUniformMat4f("model", m_model);
			m_LightBox->SetUniformVec3("lightColor", LightColor[i]);

			m_VertexArray->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
			m_VertexArray->UnBind();
		}
	}

	void DeferredShading::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void DeferredShading::updateDeferredShadingVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void DeferredShading::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& DeferredShading::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}