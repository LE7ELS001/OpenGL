#include "InstanceTest.h"

namespace test {
	InstanceTest::InstanceTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(INST_YAW), Pitch(INST_PITCH), Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(INST_SPEED), MouseSensitivity(INST_SENSITIVITY), Zoom(INST_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), lightPos(1.2f, 1.0f, 2.0f), matrixmove(0), RockMeshSize(0)
	{
		{
			m_window = window;

			updateLightVectors();

			GLCall(glEnable(GL_DEPTH_TEST));



			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




			//加载着色器
			m_ShaderRock = std::make_unique<Shader>("src/res/Shaders/RockTest.shader");
			m_ShaderPlanet = std::make_unique<Shader>("src/res/Shaders/Planet.shader");
			m_ShaderPlanet->Bind();
			m_ShaderPlanet->SetUniform1i("texture_diffuse1", 0);
			


			//陨石模型加载
			std::string Rockpath = "src/res/Model/rock/rock.obj";
			m_ModelRock = std::make_unique<Model>(Rockpath, false);
			
			//星球模型加载
			std::string Planetpath = "src/res/Model/planet/planet.obj";
			m_ModelPlanet = std::make_unique<Model>(Planetpath, false);


			unsigned int amount = 10000;
			glm::mat4* modelMatrices;
			modelMatrices = new glm::mat4[amount];
			srand(static_cast<unsigned int>(glfwGetTime())); 
			float radius = 150.0;
			float offset = 25.0f;
			for (unsigned int i = 0; i < amount; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				float angle = (float)i / (float)amount * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float y = displacement * 0.4f; 
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float z = cos(angle) * radius + displacement;
				model = glm::translate(model, glm::vec3(x, y, z));

				
				float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
				model = glm::scale(model, glm::vec3(scale));

				
				float rotAngle = static_cast<float>((rand() % 360));
				model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

				
				modelMatrices[i] = model;
			}

			m_InstanceVBO = std::make_unique<VertexBuffer>(&modelMatrices[0], 10000 * sizeof(glm::mat4));
			int meshSize = m_ModelRock->meshes.size();
			RockMeshSize = meshSize;
			


			for (unsigned int i = 0; i < meshSize; i++)
			{
				unsigned int VAO = m_ModelRock->meshes[i].VAO;
				GLCall(glBindVertexArray(VAO));
				
				//mat4好像是4个size的vec4
				GLCall(glEnableVertexAttribArray(3));
				GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0));
				
				GLCall(glEnableVertexAttribArray(4));
				GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))));
				
				GLCall(glEnableVertexAttribArray(5));
				GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))));
				
				GLCall(glEnableVertexAttribArray(6));
				GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4))));
				
				GLCall(glVertexAttribDivisor(3, 1));
				GLCall(glVertexAttribDivisor(4, 1));
				GLCall(glVertexAttribDivisor(5, 1));
				GLCall(glVertexAttribDivisor(6, 1));
				
				glBindVertexArray(0);
				
			}


		}
	}

	InstanceTest::InstanceTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateLightVectors();
	}



	InstanceTest::~InstanceTest()
	{
	}

	glm::mat4 InstanceTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void InstanceTest::ProcessKeyboard(InstanceTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == INST_FORWARD)
			Position += Front * velocity;
		if (direction == INST_BACKWARD)
			Position -= Front * velocity;
		if (direction == INST_LEFT)
			Position -= Right * velocity;
		if (direction == INST_RIGHT)
			Position += Right * velocity;
	}

	void InstanceTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(INST_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(INST_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(INST_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(INST_RIGHT, deltaTime);
	}

	void InstanceTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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

	void InstanceTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void InstanceTest::ScrolINST_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void InstanceTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		InstanceTest* instance = static_cast<InstanceTest*>(glfwGetWindowUserPointer(window));
		instance->ScrolINST_callback(window, xoffset, yoffset);
	}

	void InstanceTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void InstanceTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void InstanceTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		InstanceTest* instance = static_cast<InstanceTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void InstanceTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void InstanceTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	void InstanceTest::OnUpdate(float deltaTime)
	{

	}

	void InstanceTest::OnRender()
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


		m_proj = glm::perspective(glm::radians(45.0f), (float)(960 / 540), 0.1f, 1000.0f);
		m_view = GetViewMatrix();
		
		m_ShaderRock->Bind();
		m_ShaderRock->SetUniformMat4f("projection", m_proj);
		m_ShaderRock->SetUniformMat4f("view", m_view);

		m_ShaderRock->SetUniform1f("shininess", 32.0f);
		m_ShaderRock->SetUniformVec3("viewPos", Position);
		m_ShaderRock->SetUniformVec3("dirLight.direction", 1.0f, 1.0f, 0.3f);
		m_ShaderRock->SetUniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderRock->SetUniformVec3("dirLight.diffuse", 0.9f, 0.9f, 0.9f);
		m_ShaderRock->SetUniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);


		m_ShaderPlanet->Bind();
		m_ShaderPlanet->SetUniformMat4f("projection", m_proj);
		m_ShaderPlanet->SetUniformMat4f("view", m_view);

		m_ShaderPlanet->SetUniform1f("shininess", 32.0f);
		m_ShaderPlanet->SetUniformVec3("viewPos", Position);
		m_ShaderPlanet->SetUniformVec3("dirLight.direction", 1.0f, 1.0f, 0.3f);
		m_ShaderPlanet->SetUniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_ShaderPlanet->SetUniformVec3("dirLight.diffuse", 0.9f, 0.9f, 0.9f);
		m_ShaderPlanet->SetUniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		
		m_model = glm::mat4(1.0);
		m_model = glm::translate(m_model, glm::vec3(0.0f, -3.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(24.0f, 24.0f, 24.0f));
		m_ShaderPlanet->SetUniformMat4f("model", m_model);
		
		m_ModelPlanet->Draw(*m_ShaderPlanet.get());
		

		

		m_ShaderRock->Bind();
		GLCall(glActiveTexture(GL_TEXTURE0));
		m_ShaderRock->SetUniform1i("texture_diffuse1",0);
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ModelRock->textures_loaded[0].ID));
		for (unsigned int i = 0; i < RockMeshSize; i++)
		{
			GLCall(glBindVertexArray(m_ModelRock->meshes[i].VAO));
			GLCall(glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(m_ModelRock->meshes[i].Me_indices.size()), GL_UNSIGNED_INT, 0, 10000));
			GLCall(glBindVertexArray(0));
		}
		


	}

	void InstanceTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void InstanceTest::updateLightVectors()
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