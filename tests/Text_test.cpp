#include "Text_test.h"

namespace test {
	Text_test::Text_test(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(TT_YAW), Pitch(TT_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(TT_SPEED), MouseSensitivity(TT_SENSITIVITY), Zoom(TT_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f),
		LightPos(0.0, 0.0, 0.0), LightProjection(glm::mat4(1.0f)), LightView(glm::mat4(1.0f)), LightSpaceMartrix(glm::mat4(1.0f)), ShadowON(false), ShadowKey(false)
	{
		{
			m_window = window;
			updateText_testVectors();

			GLCall(glViewport(0, 0, 960, 540));

			//GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_text = std::make_unique<Text>();



		}


	}


	Text_test::Text_test(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateText_testVectors();
	}



	Text_test::~Text_test()
	{
	}

	glm::mat4 Text_test::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Text_test::ProcessKeyboard(My_Text_test_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == TT_FORWARD)
			Position += Front * velocity;
		if (direction == TT_BACKWARD)
			Position -= Front * velocity;
		if (direction == TT_LEFT)
			Position -= Right * velocity;
		if (direction == TT_RIGHT)
			Position += Right * velocity;
	}

	void Text_test::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(TT_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(TT_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(TT_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(TT_RIGHT, deltaTime);

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

	void Text_test::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateText_testVectors();
	}

	void Text_test::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void Text_test::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void Text_test::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Text_test* instance = static_cast<Text_test*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void Text_test::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void Text_test::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void Text_test::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Text_test* instance = static_cast<Text_test*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void Text_test::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void Text_test::OnUpdate(float deltaTime)
	{

	}

	void Text_test::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		//glfwPollEvents();
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_text->RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
		m_text->RenderText("Opengl(c)", 500.0f, 500.0f, 0.5f, glm::vec3(0.3f, 0.7f, 0.9f));
		


		
	}

	void Text_test::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void Text_test::updateText_testVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	//分情况绘制 一种是为了得到深度的绘制 一种是最终输出画面的绘制 取决用什么shader
	void Text_test::RenderScene(Shader& shader)
	{
		//地板绘制
		glm::mat4 model = glm::mat4(1.0f);
		shader.SetUniformMat4f("model", model);
		m_PlaneVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		m_PlaneVertexArray->UnBind();

		//方块绘制
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		shader.SetUniformMat4f("model", model);
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
		shader.SetUniformMat4f("model", model);
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
		model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", model);
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

	}

	void Text_test::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& Text_test::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}