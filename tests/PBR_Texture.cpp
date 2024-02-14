#include "PBR_Texture.h"

namespace test {
	PBR_Texture::PBR_Texture(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(PBR_Texture_YAW), Pitch(PBR_Texture_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(PBR_Texture_SPEED), MouseSensitivity(PBR_Texture_SENSITIVITY), Zoom(PBR_Texture_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), LightPos(2.0f, 4.0f, -2.0f), blinn(false), blinnKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updatePBR_TextureVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小

			//stbi_set_flip_vertically_on_load(true);
			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



			//球
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
				{
					float xSegment = (float)x / (float)X_SEGMENTS;
					float ySegment = (float)y / (float)Y_SEGMENTS;
					float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
					float yPos = std::cos(ySegment * PI);
					float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

					positions.push_back(glm::vec3(xPos, yPos, zPos));
					uv.push_back(glm::vec2(xSegment, ySegment));
					normals.push_back(glm::vec3(xPos, yPos, zPos));
				}
			}


			bool oddRow = false;
			for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
			{
				if (!oddRow)
				{
					for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
					{
						indices.push_back(y * (X_SEGMENTS + 1) + x);
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					}
				}
				else
				{
					for (int x = X_SEGMENTS; x >= 0; --x)
					{
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
						indices.push_back(y * (X_SEGMENTS + 1) + x);
					}
				}
				oddRow = !oddRow;
			}
			indiceCount = static_cast<unsigned int>(indices.size());

			for (unsigned int i = 0; i < positions.size(); ++i)
			{
				m_data.push_back(positions[i].x);
				m_data.push_back(positions[i].y);
				m_data.push_back(positions[i].z);
				if (normals.size() > 0)
				{
					m_data.push_back(normals[i].x);
					m_data.push_back(normals[i].y);
					m_data.push_back(normals[i].z);
				}
				if (uv.size() > 0)
				{
					m_data.push_back(uv[i].x);
					m_data.push_back(uv[i].y);
				}
			}



			//Shader
			m_Shader = std::make_unique<Shader>("src/res/Shaders/PBR/PBR_Texture.shader");

			m_Shader->Bind();
			m_Shader->SetUniform1i("albedoMap", 0);
			m_Shader->SetUniform1i("normalMap", 1);
			m_Shader->SetUniform1i("metallicMap", 2);
			m_Shader->SetUniform1i("roughnessMap", 3);
			m_Shader->SetUniform1i("aoMap", 4);

			m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
			m_Shader->SetUniformMat4f("projection", m_proj);

			//Texture
			m_albedo = std::make_unique<TextureGamma>("src/res/Picture/PBR_Texture/DIFF.jpg", true);
			m_normal = std::make_unique<TextureGamma>("src/res/Picture/PBR_Texture/NOR", true);
			m_metallic = std::make_unique<TextureGamma>("src/res/Picture/PBR_Texture/META.jpg", true);
			m_roughness = std::make_unique<TextureGamma>("src/res/Picture/PBR_Texture/ROUGH.jpg", true);
			m_ao = std::make_unique<TextureGamma>("src/res/Picture/PBR_Texture/AO.jpg", true);

			//VAB VBO
			m_VertexArray = std::make_unique<VertexArray>();
			m_VertexBuffer = std::make_unique<VertexBuffer>(&m_data[0], m_data.size() * sizeof(float));
			m_IndexBuffer = std::make_unique<IndexBuffer>(&indices[0], indices.size());
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArray->AddBuffer(*m_VertexBuffer, layout);


		}


	}


	PBR_Texture::PBR_Texture(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updatePBR_TextureVectors();
	}



	PBR_Texture::~PBR_Texture()
	{
	}

	glm::mat4 PBR_Texture::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void PBR_Texture::ProcessKeyboard(My_PBR_Texture_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == PBR_Texture_FORWARD)
			Position += Front * velocity;
		if (direction == PBR_Texture_BACKWARD)
			Position -= Front * velocity;
		if (direction == PBR_Texture_LEFT)
			Position -= Right * velocity;
		if (direction == PBR_Texture_RIGHT)
			Position += Right * velocity;
	}

	void PBR_Texture::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(PBR_Texture_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(PBR_Texture_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(PBR_Texture_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(PBR_Texture_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKey)
		{
			blinn = !blinn;
			blinnKey = true;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			blinnKey = false;
		}
	}

	void PBR_Texture::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updatePBR_TextureVectors();
	}

	void PBR_Texture::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void PBR_Texture::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void PBR_Texture::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		PBR_Texture* instance = static_cast<PBR_Texture*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void PBR_Texture::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void PBR_Texture::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void PBR_Texture::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		PBR_Texture* instance = static_cast<PBR_Texture*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void PBR_Texture::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	float PBR_Texture::ourLerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	void PBR_Texture::OnUpdate(float deltaTime)
	{

	}

	void PBR_Texture::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		m_view = GetViewMatrix();
		m_model = glm::mat4(1.0f);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("view", m_view);
		m_Shader->SetUniformVec3("camPos", Position);

		m_albedo->Bind(0);
		m_normal->Bind(1);
		m_metallic->Bind(2);
		m_roughness->Bind(3);
		m_ao->Bind(4);


		//球体
		for (int row = 0; row < nrRows; ++row)
		{
			m_Shader->SetUniform1f("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColums; ++col)
			{
				m_Shader->SetUniform1f("roughness", glm::clamp((float)col / (float)nrColums, 0.05f, 1.0f));

				m_model = glm::mat4(1.0f);
				m_model = glm::translate(m_model, glm::vec3((col - (nrColums / 2)) * space,
					(row - (nrRows / 2)) * space,
					0.0f
				));
				m_Shader->SetUniformMat4f("model", m_model);
				m_Shader->SetUniformMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(m_model))));
				m_VertexArray->Bind();
				m_IndexBuffer->Bind();
				GLCall(glDrawElements(GL_TRIANGLE_STRIP, indiceCount, GL_UNSIGNED_INT, 0));
				m_VertexArray->UnBind();
				m_IndexBuffer->UnBind();
			}
		}



		//光源
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			m_Shader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			m_Shader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			m_model = glm::mat4(1.0f);
			m_model = glm::translate(m_model, newPos);
			m_model = glm::scale(m_model, glm::vec3(0.5f));
			m_Shader->SetUniformMat4f("model", m_model);
			m_Shader->SetUniformMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(m_model))));
			m_VertexArray->Bind();
			m_IndexBuffer->Bind();
			GLCall(glDrawElements(GL_TRIANGLE_STRIP, indiceCount, GL_UNSIGNED_INT, 0));
			m_VertexArray->UnBind();
		}



	}

	void PBR_Texture::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void PBR_Texture::updatePBR_TextureVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void PBR_Texture::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& PBR_Texture::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}