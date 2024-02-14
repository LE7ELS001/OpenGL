#include "SkyboxTest.h"

namespace test {
	SkyboxTest::SkyboxTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(SK_YAW), Pitch(SK_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SK_SPEED), MouseSensitivity(SK_SENSITIVITY), Zoom(SK_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateSkyboxTestVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小


			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			
			float cubeVertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			   
			   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			   
			   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			   
			    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			   
			   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			   
			   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
			};
			float skyboxVertices[] = {         
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};

			m_ShaderCube = std::make_unique<Shader>("src/res/Shaders/CubeMapCube.shader");
			m_ShaderSkyBox = std::make_unique<Shader>("src/res/Shaders/CubeMap.shader");



			m_VertexArrayCube = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(cubeVertices, 6 * 36 * sizeof(float));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			m_VertexArrayCube->AddBuffer(*m_VertexbBuffer, layout);


			m_VertexArraySkyBox = std::make_unique<VertexArray>();
			m_vertexBuffer2 = std::make_unique<VertexBuffer>(skyboxVertices, 6 * 18 * sizeof(float));
			VertexBufferLayout layout2;
			layout2.push<float>(3);
			m_VertexArraySkyBox->AddBuffer(*m_vertexBuffer2, layout2);

			//m_TextureCube = std::make_unique<Texture>("src/res/Picture/container.jpg"); 因为映射的是天空盒 所以不需要自身材质了

			m_SkyBox = std::make_unique<CubeMap>("src/res/Picture/skybox/right.jpg",
												 "src/res/Picture/skybox/left.jpg",
												 "src/res/Picture/skybox/top.jpg",
												 "src/res/Picture/skybox/bottom.jpg",
												 "src/res/Picture/skybox/front.jpg",
												 "src/res/Picture/skybox/back.jpg");

			m_ShaderCube->Bind();
			m_ShaderCube->SetUniform1i("skybox", 0);

			m_ShaderSkyBox->Bind();
			m_ShaderSkyBox->SetUniform1i("skybox", 0);
		}


	}


	SkyboxTest::SkyboxTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateSkyboxTestVectors();
	}



	SkyboxTest::~SkyboxTest()
	{
	}

	glm::mat4 SkyboxTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void SkyboxTest::ProcessKeyboard(My_SkyboxTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == SK_FORWARD)
			Position += Front * velocity;
		if (direction == SK_BACKWARD)
			Position -= Front * velocity;
		if (direction == SK_LEFT)
			Position -= Right * velocity;
		if (direction == SK_RIGHT)
			Position += Right * velocity;
	}

	void SkyboxTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(SK_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(SK_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(SK_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(SK_RIGHT, deltaTime);
	}

	void SkyboxTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateSkyboxTestVectors();
	}

	void SkyboxTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void SkyboxTest::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void SkyboxTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		SkyboxTest* instance = static_cast<SkyboxTest*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void SkyboxTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void SkyboxTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void SkyboxTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		SkyboxTest* instance = static_cast<SkyboxTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void SkyboxTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void SkyboxTest::OnUpdate(float deltaTime)
	{

	}

	void SkyboxTest::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_ShaderCube->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom),(float)(960/540), 0.1f, 100.0f);
		m_ShaderCube->SetUniformMat4f("model", model);
		m_ShaderCube->SetUniformMat4f("projection", m_proj);
		m_ShaderCube->SetUniformMat4f("view", m_view);
		m_ShaderCube->SetUniformVec3("cameraPos", Position);

		//方块盒子
		m_VertexArrayCube->Bind();
		//m_TextureCube->Bind();
		m_SkyBox->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_VertexArrayCube->UnBind();

		GLCall(glDepthFunc(GL_LEQUAL)); //改变深度测试 小于等于1的都绘制 也就是小于天空盒
		m_ShaderSkyBox->Bind();
		m_view = glm::mat4(glm::mat3(GetViewMatrix())); //这一步好像是因为 不希望立方体贴图随着摄像机移动而移动 这一步会移除所有的平移 保留旋转 
		m_ShaderSkyBox->SetUniformMat4f("view", m_view);
		m_ShaderSkyBox->SetUniformMat4f("projection", m_proj);

		m_VertexArraySkyBox->Bind();
		m_SkyBox->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_VertexArraySkyBox->UnBind();
		GLCall(glDepthFunc(GL_LESS));//深度测试设置为默认
	}

	void SkyboxTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void SkyboxTest::updateSkyboxTestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void SkyboxTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& SkyboxTest::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}