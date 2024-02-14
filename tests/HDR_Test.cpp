#include "HDR_Test.h"

namespace test {
	HDR_Test::HDR_Test(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 5.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(HDR_YAW), Pitch(HDR_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(HDR_SPEED), MouseSensitivity(HDR_SENSITIVITY), Zoom(HDR_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f),  gammaEnable(true), gammaKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateHDR_TestVectors();


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
			m_Shader = std::make_unique<Shader>("src/res/Shaders/HDR/Light.shader");
			m_HDR_Shader = std::make_unique<Shader>("src/res/Shaders/HDR/HDR.shader");
			
			m_Shader->Bind();
			m_Shader->SetUniform1i("diffuseTexture", 0);

			m_HDR_Shader->Bind();
			m_HDR_Shader->SetUniform1i("hdrBuffer", 0);


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
			m_HDR_FBO = std::make_unique<HDR_FBO>();


			//纹理
			m_texture = std::make_unique<TextureGamma>("src/res/Picture/wood.png",true);

			//光源位置 
			LightPosition.push_back(glm::vec3(0.0f, 0.0f, 49.5f));
			LightPosition.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
			LightPosition.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
			LightPosition.push_back(glm::vec3(0.8f, -1.7f, 6.0f));

			//光源颜色
			LightColor.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
			LightColor.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
			LightColor.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
			LightColor.push_back(glm::vec3(0.0f, 0.1f, 0.0f));


			



		}


	}


	HDR_Test::HDR_Test(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateHDR_TestVectors();
	}



	HDR_Test::~HDR_Test()
	{
	}

	glm::mat4 HDR_Test::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void HDR_Test::ProcessKeyboard(My_HDR_Test_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == HDR_FORWARD)
			Position += Front * velocity;
		if (direction == HDR_BACKWARD)
			Position -= Front * velocity;
		if (direction == HDR_LEFT)
			Position -= Right * velocity;
		if (direction == HDR_RIGHT)
			Position += Right * velocity;
	}

	void HDR_Test::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(HDR_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(HDR_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(HDR_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(HDR_RIGHT, deltaTime);

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

	void HDR_Test::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateHDR_TestVectors();
	}

	void HDR_Test::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void HDR_Test::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void HDR_Test::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		HDR_Test* instance = static_cast<HDR_Test*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void HDR_Test::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void HDR_Test::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void HDR_Test::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		HDR_Test* instance = static_cast<HDR_Test*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void HDR_Test::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void HDR_Test::OnUpdate(float deltaTime)
	{

	}

	void HDR_Test::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		m_HDR_FBO->Bind();
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 25.0f));
		m_model = glm::scale(m_model, glm::vec3(2.5f, 2.5f, 27.5f));
		m_Shader->SetUniformMat4f("model", m_model);
		m_Shader->SetUniform1i("inverse_normals", true);

		m_texture->Bind();

		for (unsigned int  i = 0; i < LightPosition.size(); i++)
		{
			m_Shader->SetUniformVec3("lights[" + std::to_string(i) + "].Position", LightPosition[i]);
			m_Shader->SetUniformVec3("lights[" + std::to_string(i) + "].Color", LightColor[i]);
		}

		m_Shader->SetUniformVec3("viewPos", Position);

		

		//绘制光源
		m_VertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_VertexArray->UnBind();


		//转回默认帧缓冲
		
		m_HDR_FBO->UnBind();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		
		
		m_HDR_Shader->Bind();
		//m_HDR_FBO->BindColorBuffer();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_HDR_FBO->GetColor());
		m_HDR_Shader->SetUniform1i("hdr", gammaEnable);
		m_HDR_Shader->SetUniform1f("exposure", exposure);
		
		//绘制通道
		m_VertexArrayQuad->Bind();
		GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		m_VertexArrayQuad->UnBind();


		
		

		
	}

	void HDR_Test::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void HDR_Test::updateHDR_TestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void HDR_Test::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& HDR_Test::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}