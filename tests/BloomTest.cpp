#include "BloomTest.h"

namespace test {
	BloomTest::BloomTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 5.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(BLO_YAW), Pitch(BLO_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(BLO_SPEED), MouseSensitivity(BLO_SENSITIVITY), Zoom(BLO_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), gammaEnable(true), gammaKey(false), horizontal(true), first_iteration(true)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateBloomTestVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小
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

			//shader
			m_Shader = std::make_unique<Shader>("src/res/Shaders/Bloom/Bloom.shader");
			m_LightShader = std::make_unique<Shader>("src/res/Shaders/Bloom/BloomLight.shader");
			m_BlurShader = std::make_unique<Shader>("src/res/Shaders/Bloom/Blur.shader");
			m_FinallyShader = std::make_unique<Shader>("src/res/Shaders/Bloom/Finally.shader");

			m_Shader->Bind();
			m_Shader->SetUniform1i("diffuseTexture", 0);

			m_BlurShader->Bind();
			m_BlurShader->SetUniform1i("image", 0);

			m_FinallyShader->Bind();
			m_FinallyShader->SetUniform1i("scene", 0);
			m_FinallyShader->SetUniform1i("bloomBlur", 1);



			

			//VAO VBO
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


			//HDR_FBO
			m_HDR_FBO = std::make_unique<HDR_FBO2>();

			//PingPong FBO
			m_PingPong1 = std::make_unique<PingPong>();
			m_PingPong2 = std::make_unique<PingPong>();
			


			//纹理
			m_textureWood = std::make_unique<TextureGamma>("src/res/Picture/wood.png", true);
			m_textureContainer = std::make_unique<TextureGamma>("src/res/Picture/container2.png", true);

			//光源位置 
			LightPosition.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
			LightPosition.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
			LightPosition.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
			LightPosition.push_back(glm::vec3(-0.8f, 2.4f, -1.0f));

			//光源颜色
			LightColor.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
			LightColor.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
			LightColor.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
			LightColor.push_back(glm::vec3(0.0f, 5.0f, 0.0f));






		}


	}


	BloomTest::BloomTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateBloomTestVectors();
	}



	BloomTest::~BloomTest()
	{
	}

	glm::mat4 BloomTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void BloomTest::ProcessKeyboard(My_BloomTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == BLO_FORWARD)
			Position += Front * velocity;
		if (direction == BLO_BACKWARD)
			Position -= Front * velocity;
		if (direction == BLO_LEFT)
			Position -= Right * velocity;
		if (direction == BLO_RIGHT)
			Position += Right * velocity;
	}

	void BloomTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(BLO_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(BLO_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(BLO_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(BLO_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !gammaKey)
		{
			gammaEnable = !gammaEnable;
			gammaKey = true;
			std::cout << "HDR : " << gammaEnable << std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			gammaKey = false;
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			if (exposure > 0.0f)
			{
				exposure -= 0.001f;
			}
			else
			{
				exposure = 0.0f;
			}
			std::cout << "exposure : " << exposure << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			exposure += 0.001f;
			std::cout << "exposure : " << exposure << std::endl;
		}
	}

	void BloomTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateBloomTestVectors();
	}

	void BloomTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void BloomTest::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void BloomTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		BloomTest* instance = static_cast<BloomTest*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void BloomTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void BloomTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void BloomTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		BloomTest* instance = static_cast<BloomTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void BloomTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void BloomTest::OnUpdate(float deltaTime)
	{

	}

	void BloomTest::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_HDR_FBO->Bind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);

		m_textureWood->Bind(0);

		for (unsigned int i = 0; i < LightPosition.size(); i++)
		{
			m_Shader->SetUniformVec3("lights[" + std::to_string(i) + "].Position", LightPosition[i]);
			m_Shader->SetUniformVec3("lights[" + std::to_string(i) + "].Color", LightColor[i]);
		}

		m_Shader->SetUniformVec3("viewPos", Position);

		//绘制地板
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0f, -1.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(12.5f, 0.5f, 12.5f));
		m_Shader->SetUniformMat4f("model", m_model);
		m_VertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		//m_VertexArray->UnBind();

		//绘制立方体
		//1
		m_textureContainer->Bind();
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0f, 1.5f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		m_Shader->SetUniformMat4f("model", m_model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//2
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(2.0f, 0.0f, 1.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		m_Shader->SetUniformMat4f("model", m_model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//3
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-1.0f, -1.0f, 2.0f));
		m_model = glm::rotate(m_model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		m_Shader->SetUniformMat4f("model", m_model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//4
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0f, 2.7f, 4.0f));
		m_model = glm::rotate(m_model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		m_model = glm::scale(m_model, glm::vec3(1.25f));
		m_Shader->SetUniformMat4f("model", m_model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//5
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-2.0f, 1.0f, -3.0f));
		m_model = glm::rotate(m_model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		m_Shader->SetUniformMat4f("model", m_model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//6
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-3.0f, 0.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		m_Shader->SetUniformMat4f("model", m_model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_VertexArray->UnBind();

		//光源绘制
		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("projection", m_proj);
		m_LightShader->SetUniformMat4f("view", m_view);

		for (unsigned int  i = 0; i < LightPosition.size(); i++)
		{
			m_model = glm::mat4(1.0f);
			m_model = glm::translate(m_model, glm::vec3(LightPosition[i]));
			m_model = glm::scale(m_model, glm::vec3(0.25f));
			m_LightShader->SetUniformMat4f("model", m_model);
			m_LightShader->SetUniformVec3("lightColor", LightColor[i]);
			m_VertexArray->Bind();
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			m_VertexArray->UnBind();
		}

		//转回默认帧缓冲

		m_HDR_FBO->UnBind();
		

		//模糊光源

		m_BlurShader->Bind();
		horizontal = true;
		first_iteration = true;
		for (unsigned int i = 0; i < BlurAmount; i++)
		{
			if (first_iteration == true)
			{
					m_PingPong2->Bind();
					m_BlurShader->SetUniform1i("horizontal", true);
					GLCall(glBindTexture(GL_TEXTURE_2D, m_HDR_FBO->GetColor(1)));
					m_VertexArrayQuad->Bind();
					GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
					m_VertexArrayQuad->UnBind();
					horizontal = !horizontal;
					first_iteration = false;
			}
			else
			{
				if (horizontal == true)
				{
					m_PingPong2->Bind();
					m_BlurShader->SetUniform1i("horizontal", true);
					m_PingPong1->BindColorBuffer();
					m_VertexArrayQuad->Bind();
					GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
					m_VertexArrayQuad->UnBind();
					horizontal = !horizontal;
				}
				else
				{
					m_PingPong1->Bind();
					m_BlurShader->SetUniform1i("horizontal", false);
					m_PingPong2->BindColorBuffer();
					m_VertexArrayQuad->Bind();
					GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
					m_VertexArrayQuad->UnBind();
					horizontal = !horizontal;
				}
			}
		}


		//转会默认帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_FinallyShader->Bind();
		m_HDR_FBO->BindColorBuffer(0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, m_PingPong1->GetColor());
		m_PingPong2->BindColorBuffer(1);
		

		m_FinallyShader->SetUniform1i("bloom", gammaEnable);
		m_FinallyShader->SetUniform1f("exposure", exposure);
		
		m_VertexArrayQuad->Bind();
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		m_VertexArrayQuad->UnBind();

	}

	void BloomTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void BloomTest::updateBloomTestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void BloomTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& BloomTest::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}