#include "TestMultiLight.h"

namespace test {
	MultiLight::MultiLight(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(J_YAW), Pitch(J_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(J_SPEED), MouseSensitivity(J_SENSITIVITY), Zoom(J_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateMultiLightVectors();

			GLCall(glEnable(GL_DEPTH_TEST));

			float vertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  //位置 法向 纹理坐标
				 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
			};

			GLCall(glEnable(GL_BLEND));
			GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_VertexArrayObject = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 48  * sizeof(float)); //
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArrayObject->AddBuffer(*m_VertexbBuffer, layout);

			m_VertexArrayLightCube = std::make_unique<VertexArray>();
			m_VertexbBuffer->Bind();
			m_VertexArrayLightCube->AddBuffer(*m_VertexbBuffer, layout);


			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/MultipleLight.shader");
			m_ShaderObject->Bind();

			m_ShaderLightCube = std::make_unique<Shader>("src/res/Shaders/LightCube.shader");
			m_ShaderLightCube->Bind();

			m_Texture1 = std::make_unique<Texture>("src/res/Picture/container2.png");
			m_Texture2 = std::make_unique<Texture>("src/res/Picture/container2_specular.png");


			m_ShaderObject->Bind();
			m_ShaderObject->SetUniform1i("material.diffuse", 0);
			m_ShaderObject->SetUniform1i("material.specular", 1);










		}


	}

	MultiLight::MultiLight(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateMultiLightVectors();
	}



	MultiLight::~MultiLight()
	{
	}

	glm::mat4 MultiLight::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void MultiLight::ProcessKeyboard(My_MultiLight_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == J_FORWARD)
			Position += Front * velocity;
		if (direction == J_BACKWARD)
			Position -= Front * velocity;
		if (direction == J_LEFT)
			Position -= Right * velocity;
		if (direction == J_RIGHT)
			Position += Right * velocity;
	}

	void MultiLight::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(J_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(J_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(J_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(J_RIGHT, deltaTime);
	}

	void MultiLight::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateMultiLightVectors();
	}

	void MultiLight::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void MultiLight::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void MultiLight::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		MultiLight* instance = static_cast<MultiLight*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void MultiLight::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void MultiLight::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void MultiLight::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		MultiLight* instance = static_cast<MultiLight*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void MultiLight::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void MultiLight::OnUpdate(float deltaTime)
	{

	}

	void MultiLight::OnRender()
	{

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		Renderer renderer;

		m_ShaderObject->Bind();
		m_ShaderObject->SetUniformVec3("viewPos", Position);
		m_ShaderObject->SetUniform1f("material.shininess", 32.0f);
		
		//平行光
		m_ShaderObject->SetUniformVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		m_ShaderObject->SetUniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderObject->SetUniformVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		m_ShaderObject->SetUniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		//点光X4
		m_ShaderObject->SetUniformVec3("pointLight[0].position", pointLightPosition[0]);
		m_ShaderObject->SetUniformVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderObject->SetUniformVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
		m_ShaderObject->SetUniformVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[0].constant", 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[0].linear", 0.09f);
		m_ShaderObject->SetUniform1f("pointLight[0].quadratic", 0.032f);

		//2
		m_ShaderObject->SetUniformVec3("pointLight[1].position", pointLightPosition[1]);
		m_ShaderObject->SetUniformVec3("pointLight[1].ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderObject->SetUniformVec3("pointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
		m_ShaderObject->SetUniformVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[1].constant", 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[1].linear", 0.09f);
		m_ShaderObject->SetUniform1f("pointLight[1].quadratic", 0.032f);

		//3
		m_ShaderObject->SetUniformVec3("pointLight[2].position", pointLightPosition[2]);
		m_ShaderObject->SetUniformVec3("pointLight[2].ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderObject->SetUniformVec3("pointLight[2].diffuse", 0.8f, 0.8f, 0.8f);
		m_ShaderObject->SetUniformVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[2].constant", 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[2].linear", 0.09f);
		m_ShaderObject->SetUniform1f("pointLight[2].quadratic", 0.032f);


		//4
		m_ShaderObject->SetUniformVec3("pointLight[3].position", pointLightPosition[3]);
		m_ShaderObject->SetUniformVec3("pointLight[3].ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderObject->SetUniformVec3("pointLight[3].diffuse", 0.8f, 0.8f, 0.8f);
		m_ShaderObject->SetUniformVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[3].constant", 1.0f);
		m_ShaderObject->SetUniform1f("pointLight[3].linear", 0.09f);
		m_ShaderObject->SetUniform1f("pointLight[3].quadratic", 0.032f);

		//聚光
		m_ShaderObject->SetUniformVec3("spotLight.position", Position);
		m_ShaderObject->SetUniformVec3("spotLight.direction", Front);
		m_ShaderObject->SetUniformVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		m_ShaderObject->SetUniformVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		m_ShaderObject->SetUniformVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		m_ShaderObject->SetUniform1f("spotLight.constant", 1.0f);
		m_ShaderObject->SetUniform1f("spotLight.linear", 0.09f);
		m_ShaderObject->SetUniform1f("spotLight.quadratic", 0.032f);
		m_ShaderObject->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		m_ShaderObject->SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);

		glm::mat4 model = glm::mat4(1.0f);
		m_ShaderObject->SetUniformMat4f("model", model);

		
		m_Texture1->Bind();
		m_Texture2->Bind(1);

		m_VertexArrayObject->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_ShaderObject->SetUniformMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		m_ShaderLightCube->Bind();

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPosition[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			m_ShaderLightCube->SetUniformMat4f("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		







	}

	void MultiLight::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void MultiLight::updateMultiLightVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void MultiLight::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}
}