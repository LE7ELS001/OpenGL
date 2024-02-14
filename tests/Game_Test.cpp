#include "Game_Test.h"

namespace test {
	Game_test::Game_test(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(GAT_YAW), Pitch(GAT_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(GAT_SPEED), MouseSensitivity(GAT_SENSITIVITY), Zoom(GAT_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f),
		LightPos(0.0, 0.0, 0.0), LightProjection(glm::mat4(1.0f)), LightView(glm::mat4(1.0f)), LightSpaceMartrix(glm::mat4(1.0f)), ShadowON(false), ShadowKey(false)
	{
		{
			m_window = window;
			//updateGame_testVectors();

			GLCall(glViewport(0, 0, 960, 540));

			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			SetKeyCallback(m_window);
			Game = std::make_unique<GameMain>(WIDTH, HEIGHT);
			Game->Init();
			






		}


	}


	Game_test::Game_test(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateGame_testVectors();
	}



	Game_test::~Game_test()
	{
	}

	glm::mat4 Game_test::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Game_test::ProcessKeyboard(My_Game_test_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == GAT_FORWARD)
			Position += Front * velocity;
		if (direction == GAT_BACKWARD)
			Position -= Front * velocity;
		if (direction == GAT_LEFT)
			Position -= Right * velocity;
		if (direction == GAT_RIGHT)
			Position += Right * velocity;
	}

	void Game_test::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(GAT_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(GAT_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(GAT_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(GAT_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !ShadowKey)
		{
			ShadowON = !ShadowON;
			ShadowKey = true;
			std::cout << ShadowON << std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			ShadowKey = false;
		}


	}

	void Game_test::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateGame_testVectors();
	}

	void Game_test::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void Game_test::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void Game_test::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game_test* instance = static_cast<Game_test*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void Game_test::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void Game_test::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void Game_test::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Game_test* instance = static_cast<Game_test*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void Game_test::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void Game_test::OnUpdate(float deltaTime)
	{

	}

	void Game_test::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		Game->ProcessInput(deltaTime);
		//activeCursorPosCallback(m_window);
		//ActiveScrollCallback(m_window);
		//ProcessKeyBoardInput(m_window, deltaTime);
		Game->Update(deltaTime);

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));



		Game->Render();








	}

	void Game_test::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void Game_test::updateGame_testVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}



	void Game_test::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& Game_test::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}

	void Game_test::Key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				Game->Keys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				Game->Keys[key] = false;
				Game->KeysProcessed[key] = false;
			}
		}
	}

	void Game_test::SetKeyCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
			Game_test* game = static_cast<Game_test*>(glfwGetWindowUserPointer(window));
		if (game) {
			game->Key_callback(window, key, scancode, action, mode); // µ÷ÓÃKey_callback
		}
			});

	}
}
