#include "TestCamera.h"

namespace test {
	Camera::Camera(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),Yaw(YAW), Pitch(PITCH), Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateCameraVectors();

		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
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

		GLCall(glEnable(GL_BLEND));
		GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexbBuffer = std::make_unique<VertexBuffer>(vertices, 30 * 6 * sizeof(float)); //
		VertexBufferLayout layout;

		layout.push<float>(3);
		layout.push<float>(2);

		m_VertexArray->AddBuffer(*m_VertexbBuffer, layout);

		m_Shader = std::make_unique<Shader>("src/res/Shaders/Camera.shader");
		m_Shader->Bind();

		m_Texture1 = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
		m_Texture2 = std::make_unique<Texture>("src/res/Picture/container.jpg");
		


		m_Shader->SetUniform1i("texture1", 0);
		m_Shader->SetUniform1i("texture2", 1);

		








		}

		
	}

	 Camera::Camera(GLFWwindow* window,float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		 m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	
	

	Camera::~Camera()
	{
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	void Camera::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			this->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(RIGHT, deltaTime);
	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void Camera::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void Camera::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Camera* instance = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void Camera::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	 void Camera::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	 void Camera::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	 {
		 Camera* instance = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		 instance->mouse_callback(window, xpos, ypos);

	 }

	 void Camera::activeCursorPosCallback(GLFWwindow* window)
	 {
		 glfwSetWindowUserPointer(window, this);
		 glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	 }


	void Camera::OnUpdate(float deltaTime)
	{

	}

	void Camera::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);

		Renderer renderer;

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessKeyBoardInput(m_window, deltaTime);

		m_Texture1->Bind();
		m_Texture2->Bind(1);

		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", m_proj);

		m_view = GetViewMatrix();
		m_Shader->SetUniformMat4f("view", m_view);

		m_VertexArray->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_Shader->SetUniformMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		



		



	}

	void Camera::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void Camera::updateCameraVectors()
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

