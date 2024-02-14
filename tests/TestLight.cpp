#include "TestLight.h"

namespace test {
	Light::Light(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(L_YAW), Pitch(L_PITCH), Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(L_SPEED), MouseSensitivity(L_SENSITIVITY), Zoom(L_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), lightPos(1.2f, 1.0f, 2.0f), matrixmove(0)
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


			m_ShaderLightCube = std::make_unique<Shader>("src/res/Shaders/LightCube.shader");
			m_ShaderLightCube->Bind();

			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/ObjectLightColor3.shader");
			m_ShaderObject->Bind();

			//加载漫射贴图和高光贴图 以纹理的形式
			m_diffuseMap = std::make_unique<Texture>("src/res/Picture/container2.png");
			m_specularMap = std::make_unique<Texture>("src/res/Picture/container2_specular.png");
			m_emissionMap = std::make_unique<Texture>("src/res/Picture/matrix.jpg");

			//设置纹理单元
			m_ShaderObject->SetUniform1i("material.diffuse", 0);   
			m_ShaderObject->SetUniform1i("material.specular", 1);
			m_ShaderObject->SetUniform1i("material.emission", 2);


			

		}


	}

	Light::Light(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateLightVectors();
	}



	Light::~Light()
	{
	}

	glm::mat4 Light::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Light::ProcessKeyboard(Light_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == L_FORWARD)
			Position += Front * velocity;
		if (direction == L_BACKWARD)
			Position -= Front * velocity;
		if (direction == L_LEFT)
			Position -= Right * velocity;
		if (direction == L_RIGHT)
			Position += Right * velocity;
	}

	void Light::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(L_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(L_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(L_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(L_RIGHT, deltaTime);
	}

	void Light::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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

	void Light::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void Light::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void Light::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Light* instance = static_cast<Light*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void Light::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void Light::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void Light::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Light* instance = static_cast<Light*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void Light::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void Light::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);
		
	}

	void Light::OnUpdate(float deltaTime)
	{

	}

	void Light::OnRender()
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


		//尝试矩阵代码移动和矩阵明亮变化
		float matrixlight;
		matrixlight = (1.0 + sin(glfwGetTime()) / 2 + 0.5);
		matrixmove = static_cast<float>((glfwGetTime() * 0.1));
		

		
		m_ShaderObject->Bind();
		m_ShaderObject->SetUniformVec3("light.position", lightPos);
		m_ShaderObject->SetUniformVec3("ViewPos", Position);

		//尝试矩阵代码移动和矩阵明亮变化
		m_ShaderObject->SetUniform1f("matrixlight", matrixlight);
		m_ShaderObject->SetUniform1f("matrixmove", matrixmove);

		//光源
		m_ShaderObject->SetUniformVec3("light.ambient", 0.2f,0.2f,0.2f);
		m_ShaderObject->SetUniformVec3("light.diffuse", 0.5f,0.5f,0.5f);
		m_ShaderObject->SetUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//材质
		m_diffuseMap->Bind();
		m_specularMap->Bind(1);
		m_emissionMap->Bind(2);
		m_ShaderObject->SetUniform1f("material.shineness", 51.0f); //shineness * 128


		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();
		glm::mat4 m_model = glm::mat4(1.0f);
		
		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);
		m_ShaderObject->SetUniformMat4f("model", m_model);

		m_VertexArrayObject->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//========================================================

		m_ShaderLightCube->Bind();
		m_ShaderLightCube->SetUniformMat4f("projection", m_proj);
		m_ShaderLightCube->SetUniformMat4f("view", m_view);
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, lightPos);
		m_model = glm::scale(m_model, glm::vec3(0.2f));
		m_ShaderLightCube->SetUniformMat4f("model", m_model);

		m_VertexArrayLightCube->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);











	}

	void Light::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void Light::updateLightVectors()
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