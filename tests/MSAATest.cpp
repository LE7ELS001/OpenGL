#include "MSAATest.h"

namespace test {
	MASSTest::MASSTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 0.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(MASS_YAW), Pitch(MASS_PITCH), Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(MASS_SPEED), MouseSensitivity(MASS_SENSITIVITY), Zoom(MASS_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), lightPos(1.2f, 1.0f, 2.0f), matrixmove(0), RockMeshSize(0), N_Frame(0), N_IFBO(0)
	{
		{
			m_window = window;

			updateLightVectors();

			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(GL_MULTISAMPLE);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //线框模式


			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			//立方体
			float cubeVertices[] = {      
				-0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f,  0.5f, -0.5f,
				 0.5f,  0.5f, -0.5f,
				-0.5f,  0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,

				-0.5f, -0.5f,  0.5f,
				 0.5f, -0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f,  0.5f,
				-0.5f, -0.5f,  0.5f,

				-0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f,  0.5f,
				-0.5f,  0.5f,  0.5f,

				 0.5f,  0.5f,  0.5f,
				 0.5f,  0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,

				-0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f, -0.5f,
				 0.5f, -0.5f,  0.5f,
				 0.5f, -0.5f,  0.5f,
				-0.5f, -0.5f,  0.5f,
				-0.5f, -0.5f, -0.5f,

				-0.5f,  0.5f, -0.5f,
				 0.5f,  0.5f, -0.5f,
				 0.5f,  0.5f,  0.5f,
				 0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f,  0.5f,
				-0.5f,  0.5f, -0.5f
			};

			//屏幕效果
			float quadVertices[] = { 
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};

			//加载着色器
			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/MASS.shader");
			m_ShaderScreen = std::make_unique<Shader>("src/res/Shaders/MASS_Screen.shader");

			//立方体VAO, VBO
			Object_VAO = std::make_unique<VertexArray>();
			Object_VBO = std::make_unique<VertexBuffer>(&cubeVertices, sizeof(cubeVertices));
			Object_VAO->Bind();
			VertexBufferLayout layout1;
			layout1.push<float>(3);
			Object_VAO->AddBuffer(*Object_VBO, layout1);
			
			//屏幕VAO, VBO
			Screen_VAO = std::make_unique<VertexArray>();
			Screen_VBO = std::make_unique<VertexBuffer>(&quadVertices, sizeof(quadVertices));
			Screen_VAO->Bind();
			VertexBufferLayout layout2;
			layout2.push<float>(2);
			layout2.push<float>(2);
			Screen_VAO->AddBuffer(*Screen_VBO, layout2);



			//因为我的帧缓冲的类还没完善到能控制传参决定什么附件等 所以帧缓冲额外写
			
			//多重采样帧缓冲
			//unsigned int FrameBuffer;
			//GLCall(glGenFramebuffers(1, &FrameBuffer));
			//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer));
			//
			////std::cout << MASS_Buffer << std::endl;
			////多重采样附件
			//unsigned int MASS_Attach;
			//GLCall(glGenTextures(1, &MASS_Attach));
			//GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MASS_Attach));
			//GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 900, 540, GL_TRUE));
			//GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
			//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, MASS_Attach, 0));
			//
			////RBO
			//unsigned int rbo;
			//GLCall(glGenRenderbuffers(1, &rbo));
			//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
			//GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 900, 540));
			//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
			//GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
			//
			////检查是否有附件绑定到帧缓冲
			//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//{
			//	std::cout << "ERROR::Framebuffer is incomplete" << std::endl;
			//}
			//
			//MASS_Buffer = FrameBuffer;
			//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			//
			//
			////中间过渡的帧缓冲
			//unsigned int IFBO;
			//GLCall(glGenFramebuffers(1, &IFBO));
			//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, IFBO));
			//InterFBO = IFBO;
			////std::cout << IFBO << std::endl;
			//
			////颜色附件
			//unsigned int screenTexture;
			//GLCall(glGenTextures(1, &screenTexture));
			//GLCall(glBindTexture(GL_TEXTURE_2D, screenTexture));
			//Screen_texture = screenTexture;
			//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 900, 540, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0));
			////std::cout << Screen_texture << std::endl;
			//
			////检查是否有附件绑定到帧缓冲
			//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//{
			//	std::cout << "ERROR::Framebuffer is incomplete" << std::endl;
			//}


			MASS_FRAMEBUFFER = std::make_unique<MASS_FrameBuffer>();
			InterFBO = std::make_unique<FrameBuffer>();

			N_Frame = MASS_FRAMEBUFFER->GetFrame();
			N_IFBO = InterFBO->GetFrame();
			
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

			m_ShaderScreen->Bind();
			m_ShaderScreen->SetUniform1i("screenTexture", 0);
			
		}
	}

	MASSTest::MASSTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateLightVectors();
	}



	MASSTest::~MASSTest()
	{
	}

	glm::mat4 MASSTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void MASSTest::ProcessKeyboard(MASSTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == MASS_FORWARD)
			Position += Front * velocity;
		if (direction == MASS_BACKWARD)
			Position -= Front * velocity;
		if (direction == MASS_LEFT)
			Position -= Right * velocity;
		if (direction == MASS_RIGHT)
			Position += Right * velocity;
	}

	void MASSTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(MASS_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(MASS_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(MASS_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(MASS_RIGHT, deltaTime);
	}

	void MASSTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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

	void MASSTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void MASSTest::ScrolMASS_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void MASSTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		MASSTest* instance = static_cast<MASSTest*>(glfwGetWindowUserPointer(window));
		instance->ScrolMASS_callback(window, xoffset, yoffset);
	}

	void MASSTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void MASSTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void MASSTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		MASSTest* instance = static_cast<MASSTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void MASSTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void MASSTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	void MASSTest::OnUpdate(float deltaTime)
	{

	}

	void MASSTest::OnRender()
	{
		
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		//GLCall(glEnable(GL_DEPTH_TEST));


		//先从带有多重采样的帧缓冲绘制
		MASS_FRAMEBUFFER->Bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		Renderer renderer;

		m_ShaderObject->Bind();
		Object_VAO->Bind();
		
		
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960.0 / 540.0), 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_model = glm::mat4(1.0f);

		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);
		m_ShaderObject->SetUniformMat4f("model", m_model);

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		
		
		//把带有多重采样的帧缓冲过渡到普通的帧缓冲 并且实现后期效果
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, N_Frame));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, N_IFBO));
		GLCall(glBlitFramebuffer(0, 0, 900, 540, 0, 0, 900, 540, GL_COLOR_BUFFER_BIT, GL_NEAREST));
		
		//转会到默认帧缓冲 让物体出现在屏幕上
		MASS_FRAMEBUFFER->UnBind();
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glClearColor(1.0f, 0.0f, 0.0f, 0.5f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT ));
		
		//绘制屏幕后期效果
		m_ShaderScreen->Bind();
		Screen_VAO->Bind();
	
		GLCall(glActiveTexture(GL_TEXTURE0));
		//glBindTexture(GL_TEXTURE_2D, InterFBO->GetTexture());
		InterFBO->BindTexture();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		

		
	}

	void MASSTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void MASSTest::updateLightVectors()
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