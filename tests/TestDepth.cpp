#include "TestDepth.h"

namespace test {
	TestDepth::TestDepth(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(DE_YAW), Pitch(DE_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(DE_SPEED), MouseSensitivity(DE_SENSITIVITY), Zoom(DE_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateTestDepthVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小


			GLCall(glEnable(GL_BLEND));
			GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			float cubeVertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //位置 纹理坐标（纹理坐标是为了演示GLCall(glDepthFunc(GL_ALWAYS)); 用不到）
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};
			float planeVertices[] = {
				 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
				-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
				-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

				 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
				-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
				 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
			};

			m_Shader = std::make_unique<Shader>("src/res/Shaders/DepthTest.shader");
			m_VertexArrayCube = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(cubeVertices, 6 * 30 * sizeof(float));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArrayCube->AddBuffer(*m_VertexbBuffer, layout);

				
			m_VertexArrayFloor = std::make_unique<VertexArray>();
			m_vertexBuffer2 = std::make_unique<VertexBuffer>(planeVertices, 2 * 15 * sizeof(float));
			///layout.push<float>(3);
			//layout.push<float>(2);
			m_VertexArrayFloor->AddBuffer(*m_vertexBuffer2, layout);
			

		}


	}


	TestDepth::TestDepth(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateTestDepthVectors();
	}



	TestDepth::~TestDepth()
	{
	}

	glm::mat4 TestDepth::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void TestDepth::ProcessKeyboard(My_TestDepth_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == DE_FORWARD)
			Position += Front * velocity;
		if (direction == DE_BACKWARD)
			Position -= Front * velocity;
		if (direction == DE_LEFT)
			Position -= Right * velocity;
		if (direction == DE_RIGHT)
			Position += Right * velocity;
	}

	void TestDepth::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(DE_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(DE_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(DE_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(DE_RIGHT, deltaTime);
	}

	void TestDepth::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateTestDepthVectors();
	}

	void TestDepth::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void TestDepth::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void TestDepth::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		TestDepth* instance = static_cast<TestDepth*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void TestDepth::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void TestDepth::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void TestDepth::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		TestDepth* instance = static_cast<TestDepth*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void TestDepth::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void TestDepth::OnUpdate(float deltaTime)
	{

	}

	void TestDepth::OnRender()
	{

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);


		m_Shader->Bind();
		

		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);

		//
		m_VertexArrayCube->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_Shader->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f));
		m_Shader->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//
		m_VertexArrayFloor->Bind();
		m_Shader->SetUniformMat4f("model", glm::mat4(1.0f));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		m_VertexArrayCube->UnBind();
		m_VertexArrayFloor->UnBind();
		
	}

	void TestDepth::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void TestDepth::updateTestDepthVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void TestDepth::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& TestDepth::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}