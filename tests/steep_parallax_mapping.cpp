#include "steep_parallax_mapping.h"

namespace test {
	SteepParallaxMapping::SteepParallaxMapping(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(SPM_YAW), Pitch(SPM_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPM_SPEED), MouseSensitivity(SPM_SENSITIVITY), Zoom(SPM_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), LightPos(0.0f, 0.0f, 0.0f), gammaEnable(false), gammaKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateSteepParallaxMappingVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小


			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			//创建一个平面所需的信息 并获取切线和副切线
			// 位置
			glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
			glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
			glm::vec3 pos3(1.0f, -1.0f, 0.0f);
			glm::vec3 pos4(1.0f, 1.0f, 0.0f);
			// 纹理坐标
			glm::vec2 uv1(0.0f, 1.0f);
			glm::vec2 uv2(0.0f, 0.0f);
			glm::vec2 uv3(1.0f, 0.0f);
			glm::vec2 uv4(1.0f, 1.0f);
			// 法线
			glm::vec3 nm(0.0f, 0.0f, 1.0f);

			glm::vec3 tangent1, bitangent1;
			glm::vec3 tangent2, bitangent2;

			//公式推导其实没理解
			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent1.x = f * (-deltaUV2.x * edge1.x - deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y - deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z - deltaUV1.x * edge2.z);

			edge1 = pos3 - pos1;
			edge2 = pos4 - pos1;
			deltaUV1 = uv3 - uv1;
			deltaUV2 = uv4 - uv1;

			f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent2.x = f * (-deltaUV2.x * edge1.x - deltaUV1.x * edge2.x);
			bitangent2.y = f * (-deltaUV2.x * edge1.y - deltaUV1.x * edge2.y);
			bitangent2.z = f * (-deltaUV2.x * edge1.z - deltaUV1.x * edge2.z);

			float quadVertices[] = {
				//位置						//法线			//纹理坐标	//切线								//副切线
				pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
				pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
				pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

				pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
				pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
				pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
			};

			//VAO VBO
			m_VertexArray = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(&quadVertices, sizeof(quadVertices));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			layout.push<float>(3);
			layout.push<float>(3);
			m_VertexArray->AddBuffer(*m_VertexbBuffer, layout);

			//Shader
			//m_Shader = std::make_unique<Shader>("src/res/Shaders/Steep_parallax_mapping/SteepParallaxMapping.shader");
			m_Shader = std::make_unique<Shader>("src/res/Shaders/Parallax_occlusion_mapping/Parallax_occlusion_mapping.shader");
			m_Shader->Bind();
			m_Shader->SetUniform1i("diffuseMap", 0);
			m_Shader->SetUniform1i("normalMap", 1);
			m_Shader->SetUniform1i("depthMap", 2);



			//纹理贴图和法线贴图
			m_texture = std::make_unique<Texture>("src/res/Picture/bricks2.jpg");
			m_textureNormal = std::make_unique<Texture>("src/res/Picture/bricks2_normal.jpg");
			m_textureParallax = std::make_unique<Texture>("src/res/Picture/bricks2_disp.jpg");

			//光的位置
			LightPos = glm::vec3(0.5f, 1.0f, 0.3f);






		}


	}


	SteepParallaxMapping::SteepParallaxMapping(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateSteepParallaxMappingVectors();
	}



	SteepParallaxMapping::~SteepParallaxMapping()
	{
	}

	glm::mat4 SteepParallaxMapping::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void SteepParallaxMapping::ProcessKeyboard(My_SteepParallaxMapping_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == SPM_FORWARD)
			Position += Front * velocity;
		if (direction == SPM_BACKWARD)
			Position -= Front * velocity;
		if (direction == SPM_LEFT)
			Position -= Right * velocity;
		if (direction == SPM_RIGHT)
			Position += Right * velocity;
	}

	void SteepParallaxMapping::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(SPM_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(SPM_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(SPM_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(SPM_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !gammaKey)
		{
			gammaEnable = !gammaEnable;
			gammaKey = true;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			gammaKey = false;
		}
	}

	void SteepParallaxMapping::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateSteepParallaxMappingVectors();
	}

	void SteepParallaxMapping::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void SteepParallaxMapping::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void SteepParallaxMapping::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		SteepParallaxMapping* instance = static_cast<SteepParallaxMapping*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void SteepParallaxMapping::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void SteepParallaxMapping::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void SteepParallaxMapping::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		SteepParallaxMapping* instance = static_cast<SteepParallaxMapping*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void SteepParallaxMapping::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void SteepParallaxMapping::OnUpdate(float deltaTime)
	{

	}

	void SteepParallaxMapping::OnRender()
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
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		m_model = glm::mat4(1.0f);
		m_model = glm::rotate(m_model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));


		m_Shader->Bind();
		m_Shader->SetUniformMat4f("model", m_model);
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);

		m_Shader->SetUniformVec3("lightPos", LightPos);
		m_Shader->SetUniformVec3("viewPos", Position);
		m_Shader->SetUniform1f("heightScale", heightScale);

		m_texture->Bind();
		m_textureNormal->Bind(1);
		m_textureParallax->Bind(2);

		//绘制平面
		m_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_VertexArray->UnBind();

		//绘制光源
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, LightPos);
		m_model = glm::scale(m_model, glm::vec3(0.1f));
		m_Shader->SetUniformMat4f("model", m_model);

		m_VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_VertexArray->UnBind();

	}

	void SteepParallaxMapping::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void SteepParallaxMapping::updateSteepParallaxMappingVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void SteepParallaxMapping::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& SteepParallaxMapping::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}