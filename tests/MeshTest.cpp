#include "MeshTest.h"

namespace test {
	MeshTest::MeshTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(M_YAW), Pitch(M_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(M_SPEED), MouseSensitivity(M_SENSITIVITY), Zoom(M_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateMeshTestVectors();

			stbi_set_flip_vertically_on_load(true);

			GLCall(glEnable(GL_DEPTH_TEST));



			GLCall(glEnable(GL_BLEND));
			GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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


			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/model1.shader");
			m_ShaderLightCube = std::make_unique<Shader>("src/res/Shaders/LightCube.shader");
			

			m_LightCubeArray = std::make_unique<VertexArray>();
			m_LightCubeBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 48 * sizeof(float));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_LightCubeArray->AddBuffer(*m_LightCubeBuffer, layout);


			std::string path = "src/res/Model/nanosuit/nanosuit.obj";
			m_model = std::make_unique<Model>(path, false);

			
			//获取mesh数量是多少（7） 以及每个mesh里面的texture有几个贴图（因为不是每个mesh他都是必然有diffuse， specular，height）
			
			for (unsigned int i = 0; i < m_model->meshes.size(); i++) //这里是获取模型底下有几个mesh（7）
			{

				std::cout << "Mesh " << i+1 << "  ID为" << m_model->meshes[i].matID << "\n" << std::endl; //mesh的ID获取
				for (const Me_Texture& textureTemp : m_model->meshes[m_model->meshes[i].matID].Me_texture)
				{

					std::cout << "  ID: " << textureTemp.ID << "\n  type:" << textureTemp.type << "\n  Path: " << textureTemp.path << "\n" << std::endl;
					
				}
				
			}
			if (!m_model->textures_loaded.empty()) //这里是打印检查我除重复后的容器里面有什么贴图
			{
				for (const auto& m : m_model->textures_loaded)
				{
					std::string texturename = m.type;
					std::string pathname = m.path;
					unsigned int number = m.ID;
					std::cout << number << " " << " " << texturename << " " << pathname << "\n" << std::endl;
				}
			}

		}


	}


	MeshTest::MeshTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateMeshTestVectors();
	}



	MeshTest::~MeshTest()
	{
	}

	glm::mat4 MeshTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void MeshTest::ProcessKeyboard(My_MeshLight_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == M_FORWARD)
			Position += Front * velocity;
		if (direction == M_BACKWARD)
			Position -= Front * velocity;
		if (direction == M_LEFT)
			Position -= Right * velocity;
		if (direction == M_RIGHT)
			Position += Right * velocity;
	}

	void MeshTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(M_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(M_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(M_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(M_RIGHT, deltaTime);
	}

	void MeshTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateMeshTestVectors();
	}

	void MeshTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void MeshTest::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void MeshTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		MeshTest* instance = static_cast<MeshTest*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void MeshTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void MeshTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void MeshTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		MeshTest* instance = static_cast<MeshTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void MeshTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void MeshTest::OnUpdate(float deltaTime)
	{

	}

	void MeshTest::OnRender()
	{

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);


		m_ShaderObject->Bind();
		m_ShaderObject->SetUniformVec3("viewPos", Position);
		m_ShaderObject->SetUniform1f("shininess", 32.0f);
		//m_ShaderObject->SetUniform1f("material.shininess", 32.0f);
		
		
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


		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		m_ShaderObject->SetUniformMat4f("model", model);
		m_model->Draw(*m_ShaderObject.get());

		m_LightCubeArray->Bind();
		m_ShaderLightCube->Bind();

		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();

		m_ShaderLightCube->SetUniformMat4f("projection", m_proj);
		m_ShaderLightCube->SetUniformMat4f("view", m_view);

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPosition[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			m_ShaderLightCube->SetUniformMat4f("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}









	}

	void MeshTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void MeshTest::updateMeshTestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void MeshTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& MeshTest::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}