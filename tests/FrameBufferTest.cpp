#include "FrameBufferTest.h"

namespace test {
	FrameBufferTest::FrameBufferTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(FB_YAW), Pitch(FB_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(FB_SPEED), MouseSensitivity(FB_SENSITIVITY), Zoom(FB_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), Fbuffer(0), Tbuffer(0), Rbuffer(0)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateFrameBufferTestVectors();
			
			GLCall(glEnable(GL_DEPTH_TEST));
			//glDisable(GL_CULL_FACE); 
			//GLCall(glDepthFunc(GL_LESS));
			//GLCall(glEnable(GL_STENCIL_TEST));
			//GLCall(glEnable(GL_BLEND));
			//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //线框模式


			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			float cubeVertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //位置 纹理坐标（纹理坐标是为了演示GLCall(glDepthFunc(GL_ALWAYS)); 用不到）
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
			};


			float planeVertices[] = {
				 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
				-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
				-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

				 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
				-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
				 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
			};

			

			float quadVertices[] = { 
				
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				1.0f, -1.0f,  1.0f, 0.0f,
				1.0f,  1.0f,  1.0f, 1.0f
			};




			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/grass.shader");
			m_ShaderScreen = std::make_unique<Shader>("src/res/Shaders/FrameBuffer.shader");

			m_VertexArrayCube = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(cubeVertices, 6 * 30 * sizeof(float));
			m_VertexArrayCube->Bind();
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArrayCube->AddBuffer(*m_VertexbBuffer, layout);


			m_VertexArrayFloor = std::make_unique<VertexArray>();
			m_vertexBuffer2 = std::make_unique<VertexBuffer>(planeVertices, 2 * 15 * sizeof(float));
			m_VertexArrayFloor->Bind();
			
			//layout.push<float>(3);
			//layout.push<float>(2);
			m_VertexArrayFloor->AddBuffer(*m_vertexBuffer2, layout);


			m_VertexArraySC = std::make_unique<VertexArray>();
			m_vertexBuffer3 = std::make_unique<VertexBuffer>(quadVertices, 2 * 12 * sizeof(float));
			m_VertexArraySC->Bind();
			VertexBufferLayout layout2;
			layout2.push<float>(2);
			layout2.push<float>(2);
			m_VertexArraySC->AddBuffer(*m_vertexBuffer3, layout2);


			m_texture1 = std::make_unique<Texture>("src/res/Picture/container.jpg");
			m_texture2 = std::make_unique<Texture>("src/res/Picture/metal.png");


			m_ShaderObject->Bind();
			m_ShaderObject->SetUniform1i("texturel", 0);

			m_ShaderScreen->Bind();
			m_ShaderScreen->SetUniform1i("ScreenTexture", 0);
			

			m_framebuffer = std::make_unique<FrameBuffer>();
			
			Fbuffer = m_framebuffer->GetFrame();
			Tbuffer = m_framebuffer->GetTexture();
			Rbuffer = m_framebuffer->GetRBO();
		}


	}


	FrameBufferTest::FrameBufferTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateFrameBufferTestVectors();
	}



	FrameBufferTest::~FrameBufferTest()
	{
	}

	glm::mat4 FrameBufferTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void FrameBufferTest::ProcessKeyboard(My_FrameBufferTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == FB_FORWARD)
			Position += Front * velocity;
		if (direction == FB_BACKWARD)
			Position -= Front * velocity;
		if (direction == FB_LEFT)
			Position -= Right * velocity;
		if (direction == FB_RIGHT)
			Position += Right * velocity;
	}

	void FrameBufferTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(FB_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(FB_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(FB_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(FB_RIGHT, deltaTime);
	}

	void FrameBufferTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateFrameBufferTestVectors();
	}

	void FrameBufferTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void FrameBufferTest::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void FrameBufferTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		FrameBufferTest* instance = static_cast<FrameBufferTest*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void FrameBufferTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void FrameBufferTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void FrameBufferTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		FrameBufferTest* instance = static_cast<FrameBufferTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void FrameBufferTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void FrameBufferTest::OnUpdate(float deltaTime)
	{

	}

	void FrameBufferTest::OnRender()
	{

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		
		m_framebuffer->Bind();
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));



		Renderer renderer;


		m_ShaderObject->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960.0/540.0) , 0.1f, 100.0f);
		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);


		m_VertexArrayCube->Bind();
		m_texture1->Bind();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_ShaderObject->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_ShaderObject->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		

		//绘制地板 并把地板也设置成1
		m_VertexArrayFloor->Bind();
		m_texture2->Bind();
		//glm::mat4 model2 = glm::mat4(1.0);
		//model2 = glm::translate(model2, glm::vec3(0.0f, -0.2f, 0.0f));
		m_ShaderObject->SetUniformMat4f("model", glm::mat4(1.0f));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		glBindVertexArray(0);
		//glDisable(GL_BLEND);
		

		//因为开始时绑定的时 frambuffer 而不是默认的 所以现在的绘制都在framebuffer上 现在绑会默认frambuffer
		m_framebuffer->UnBind();
		glDisable(GL_DEPTH_TEST);
		
		GLCall(glClearColor(1.0f, 0.0f, 0.0f, 0.5f)); //用于清理颜色
		
		GLCall(glClear(GL_COLOR_BUFFER_BIT ));
		
		m_ShaderScreen->Bind();
		m_VertexArraySC->Bind();
		m_framebuffer->BindTexture();
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		



	}

	void FrameBufferTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void FrameBufferTest::updateFrameBufferTestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void FrameBufferTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& FrameBufferTest::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}