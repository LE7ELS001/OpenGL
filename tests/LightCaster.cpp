#include "LightCaster.h"

namespace test {
	LightCast::LightCast(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(LC_YAW), Pitch(LC_PITCH), Front(glm::vec3(0.0f, 0.0f, -3.0f)), MovementSpeed(LC_SPEED), MouseSensitivity(LC_SENSITIVITY), Zoom(LC_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateLightVectors();

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

			//object
			m_VertexArrayObject = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 48 * sizeof(float)); //
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArrayObject->AddBuffer(*m_VertexbBuffer, layout);

			//LightCube
			m_VertexArrayLightCube = std::make_unique<VertexArray>();
			m_VertexbBuffer->Bind();
			m_VertexArrayLightCube->AddBuffer(*m_VertexbBuffer, layout);

			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/ObjectLightColor4.shader");
			m_ShaderObject->Bind();

			m_ShaderLightCube = std::make_unique<Shader>("src/res/Shaders/LightCube.shader");
			m_ShaderLightCube->Bind();


			//加载漫射贴图和高光贴图 以纹理的形式
			m_diffuseMap = std::make_unique<Texture>("src/res/Picture/container2.png");
			m_specularMap = std::make_unique<Texture>("src/res/Picture/container2_specular.png");
			

			//设置纹理单元
			m_ShaderObject->Bind();
			m_ShaderObject->SetUniform1i("material.diffuse", 0);
			m_ShaderObject->SetUniform1i("material.specular", 1);
			




		}


	}

	LightCast::LightCast(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateLightVectors();
	}



	LightCast::~LightCast()
	{
	}

	glm::mat4 LightCast::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void LightCast::ProcessKeyboard(LightCast_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == LC_FORWARD)
			Position += Front * velocity;
		if (direction == LC_BACKWARD)
			Position -= Front * velocity;
		if (direction == LC_LEFT)
			Position -= Right * velocity;
		if (direction == LC_RIGHT)
			Position += Right * velocity;
	}

	void LightCast::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(LC_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(LC_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(LC_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(LC_RIGHT, deltaTime);
	}

	void LightCast::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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

	void LightCast::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void LightCast::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void LightCast::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		LightCast* instance = static_cast<LightCast*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void LightCast::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void LightCast::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void LightCast::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		LightCast* instance = static_cast<LightCast*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void LightCast::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void LightCast::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	void LightCast::OnUpdate(float deltaTime)
	{

	}

	void LightCast::OnRender()
	{

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);

		Renderer renderer;

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessKeyBoardInput(m_window, deltaTime);



		m_ShaderObject->Bind();
		m_ShaderObject->SetUniformVec3("light.position", Position);
		m_ShaderObject->SetUniformVec3("ViewPos", Position);
		m_ShaderObject->SetUniformVec3("light.direction", Front);
		m_ShaderObject->SetUniform1f("light.cutOff", glm::cos(glm::radians(12.5f))); 
		m_ShaderObject->SetUniform1f("light.outerCutOff", glm::cos(glm::radians(17.5f)));


		//光源
		m_ShaderObject->SetUniformVec3("light.ambient", 0.1f, 0.1f, 0.1f);
		m_ShaderObject->SetUniformVec3("light.diffuse", 0.8f, 0.9f, 0.9f);
		m_ShaderObject->SetUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//公式里的三个数
		m_ShaderObject->SetUniform1f("light.constant", 1.0f);
		m_ShaderObject->SetUniform1f("light.Linear", 0.09f);
		m_ShaderObject->SetUniform1f("light.quadratic", 0.032f);

		
		m_ShaderObject->SetUniform1f("material.shineness", 32.0f); //shineness * 128


		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);
		
		glm::mat4 m_model = glm::mat4(1.0f);
		m_ShaderObject->SetUniformMat4f("model", m_model);

		
		
		//材质
		m_diffuseMap->Bind();
		m_specularMap->Bind(1);

		
		m_VertexArrayObject->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{ 

			glm::mat4 m_model2 = glm::mat4(1.0f);
			m_model2 = glm::translate(m_model2, cubePositions[i]);
			float angel = 20.0f * i;
			m_model2 = glm::rotate(m_model2, glm::radians(angel), glm::vec3(1.0f, 0.3f, 0.5f));
			m_ShaderObject->SetUniformMat4f("model", m_model2);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		};
		
			


		//========================================================

		//m_ShaderLightCube->Bind();
		//m_ShaderLightCube->SetUniformMat4f("projection", m_proj);
		//m_ShaderLightCube->SetUniformMat4f("view", m_view);
		//
		//m_model = glm::translate(m_model, lightPos);
		//m_model = glm::scale(m_model, glm::vec3(0.2f));
		//m_ShaderLightCube->SetUniformMat4f("model", m_model);
		//
		//m_VertexArrayLightCube->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);











	}

	void LightCast::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void LightCast::updateLightVectors()
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