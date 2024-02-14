#include "DrawNormal.h"

namespace test {
	DrawNormal::DrawNormal(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(GEON_YAW), Pitch(GEON_PITCH), Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(GEON_SPEED), MouseSensitivity(GEON_SENSITIVITY), Zoom(GEON_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), lightPos(1.2f, 1.0f, 2.0f), matrixmove(0)
	{
		{
			m_window = window;

			updateLightVectors();

			GLCall(glEnable(GL_DEPTH_TEST));



			GLCall(glEnable(GL_BLEND));
			GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




			//加载着色器
			m_ShaderModel = std::make_unique<Shader>("src/res/Shaders/DrawNormalModel.shader");
			m_ShaderNormal = std::make_unique<ShaderTwo>("src/res/Shaders/DrawNormal.shader");


			//加载模型
			std::string path = "src/res/Model/nanosuit/nanosuit.obj";
			m_Model = std::make_unique<Model>(path, false);









		}


	}

	DrawNormal::DrawNormal(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateLightVectors();
	}



	DrawNormal::~DrawNormal()
	{
	}

	glm::mat4 DrawNormal::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void DrawNormal::ProcessKeyboard(DrawNormal_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == GEON_FORWARD)
			Position += Front * velocity;
		if (direction == GEON_BACKWARD)
			Position -= Front * velocity;
		if (direction == GEON_LEFT)
			Position -= Right * velocity;
		if (direction == GEON_RIGHT)
			Position += Right * velocity;
	}

	void DrawNormal::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && CursorEnable == true)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			setCursorMode(window, GLFW_CURSOR_NORMAL);
			CursorEnable = false;

		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			this->ProcessKeyboard(GEON_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(GEON_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(GEON_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(GEON_RIGHT, deltaTime);
	}

	void DrawNormal::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateLightVectors();
	}

	void DrawNormal::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void DrawNormal::ScrolGEON_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void DrawNormal::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		DrawNormal* instance = static_cast<DrawNormal*>(glfwGetWindowUserPointer(window));
		instance->ScrolGEON_callback(window, xoffset, yoffset);
	}

	void DrawNormal::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void DrawNormal::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void DrawNormal::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		DrawNormal* instance = static_cast<DrawNormal*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void DrawNormal::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void DrawNormal::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	void DrawNormal::OnUpdate(float deltaTime)
	{

	}

	void DrawNormal::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);


		//Renderer renderer;

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessKeyBoardInput(m_window, deltaTime);

		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_model = glm::mat4(1.0);
		m_ShaderModel->Bind();
		m_ShaderModel->SetUniformMat4f("projection", m_proj);
		m_ShaderModel->SetUniformMat4f("view", m_view);
		m_ShaderModel->SetUniformMat4f("model", m_model);
		

		m_Model->Draw(*m_ShaderModel.get());

		m_ShaderNormal->Bind();
		m_ShaderNormal->SetUniformMat4f("projection", m_proj);
		m_ShaderNormal->SetUniformMat4f("view", m_view);
		m_ShaderNormal->SetUniformMat4f("model", m_model);

		m_Model->Draw(*m_ShaderNormal.get());









	}

	void DrawNormal::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void DrawNormal::updateLightVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
}