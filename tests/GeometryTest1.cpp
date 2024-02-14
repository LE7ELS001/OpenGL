#include "GeometryTest1.h";

namespace test {
	GeometryTestOne::GeometryTestOne(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(GEO_YAW), Pitch(GEO_PITCH), Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(GEO_SPEED), MouseSensitivity(GEO_SENSITIVITY), Zoom(GEO_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), lightPos(1.2f, 1.0f, 2.0f), matrixmove(0)
	{
		{
			m_window = window;

			updateLightVectors();

			GLCall(glEnable(GL_DEPTH_TEST));

			

			GLCall(glEnable(GL_BLEND));
			GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			
			float points[] = {
				-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 
				 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
				-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  
			};

			m_VertexArray = std::make_unique<VertexArray>();
			m_VertexBuffer = std::make_unique<VertexBuffer>(points, 20 * sizeof(float));
			VertexBufferLayout layout;
			layout.push<float>(2);
			layout.push<float>(3);
			m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

			m_Shader = std::make_unique<ShaderTwo>("src/res/Shaders/BasicGeometry.shader");
			




		}


	}

	GeometryTestOne::GeometryTestOne(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateLightVectors();
	}



	GeometryTestOne::~GeometryTestOne()
	{
	}

	glm::mat4 GeometryTestOne::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void GeometryTestOne::ProcessKeyboard(GeometryTestOne_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == GEO_FORWARD)
			Position += Front * velocity;
		if (direction == GEO_BACKWARD)
			Position -= Front * velocity;
		if (direction == GEO_LEFT)
			Position -= Right * velocity;
		if (direction == GEO_RIGHT)
			Position += Right * velocity;
	}

	void GeometryTestOne::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(GEO_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(GEO_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(GEO_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(GEO_RIGHT, deltaTime);
	}

	void GeometryTestOne::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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

	void GeometryTestOne::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void GeometryTestOne::ScrolGEO_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void GeometryTestOne::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		GeometryTestOne* instance = static_cast<GeometryTestOne*>(glfwGetWindowUserPointer(window));
		instance->ScrolGEO_callback(window, xoffset, yoffset);
	}

	void GeometryTestOne::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void GeometryTestOne::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void GeometryTestOne::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		GeometryTestOne* instance = static_cast<GeometryTestOne*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void GeometryTestOne::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void GeometryTestOne::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	void GeometryTestOne::OnUpdate(float deltaTime)
	{

	}

	void GeometryTestOne::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);


		Renderer renderer;

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessKeyBoardInput(m_window, deltaTime);


		m_Shader->Bind();
		m_VertexArray->Bind();
		glDrawArrays(GL_POINTS, 0, 4);



		
		

	}

	void GeometryTestOne::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void GeometryTestOne::updateLightVectors()
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