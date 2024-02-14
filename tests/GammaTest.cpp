#include "GammaTest.h"

namespace test {
	GammaTest::GammaTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(GMT_YAW), Pitch(GMT_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(GMT_SPEED), MouseSensitivity(GMT_SENSITIVITY), Zoom(GMT_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), LightPos(0.0f, 0.0f, 0.0f), gammaEnable(false), gammaKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateGammaTestVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小


			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			float planeVertices[] = {
				 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
				-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
				-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

				 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
				-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
				 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
			};



			m_Shader = std::make_unique<Shader>("src/res/Shaders/GammaTest.shader");

			m_VertexArray = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArray->AddBuffer(*m_VertexbBuffer, layout);

			m_texture = std::make_unique<TextureGamma>("src/res/Picture/wood.png", false);
			m_textureGamma = std::make_unique<TextureGamma>("src/res/Picture/wood.png", true);

			
			m_Shader->Bind();
			m_Shader->SetUniform1i("floorTexture", 0);




		}


	}


	GammaTest::GammaTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateGammaTestVectors();
	}



	GammaTest::~GammaTest()
	{
	}

	glm::mat4 GammaTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void GammaTest::ProcessKeyboard(My_GammaTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == GMT_FORWARD)
			Position += Front * velocity;
		if (direction == GMT_BACKWARD)
			Position -= Front * velocity;
		if (direction == GMT_LEFT)
			Position -= Right * velocity;
		if (direction == GMT_RIGHT)
			Position += Right * velocity;
	}

	void GammaTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(GMT_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(GMT_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(GMT_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(GMT_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !gammaKey)
		{
			gammaEnable = !gammaEnable;
			gammaKey = true;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			gammaKey = false;
		}
	}

	void GammaTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateGammaTestVectors();
	}

	void GammaTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void GammaTest::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void GammaTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		GammaTest* instance = static_cast<GammaTest*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void GammaTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void GammaTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void GammaTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		GammaTest* instance = static_cast<GammaTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void GammaTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void GammaTest::OnUpdate(float deltaTime)
	{

	}

	void GammaTest::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Shader->Bind();
		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);

		m_Shader->SetUniform3VecFloat("lightPositions", 4, &lightPositions[0][0]); //最后一个传参获取数组一个元素的一个变量
		m_Shader->SetUniform3VecFloat("lightColors", 4, &lightColors[0][0]);
		

		m_Shader->SetUniformVec3("viewPos" ,Position);
		m_Shader->SetUniform1i("gamma", gammaEnable);


		m_VertexArray->Bind();
		if (gammaEnable)
		{
			m_textureGamma->Bind();
		}
		else
		{
			m_texture->Bind();
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void GammaTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void GammaTest::updateGammaTestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void GammaTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& GammaTest::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}