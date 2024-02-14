#include "StencilTest.h"

namespace test {
	StencilTest::StencilTest(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(ST_YAW), Pitch(ST_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(ST_SPEED), MouseSensitivity(ST_SENSITIVITY), Zoom(ST_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateStencilTestVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glDepthFunc(GL_LESS)); 
			GLCall(glEnable(GL_STENCIL_TEST));
			


			GLCall(glEnable(GL_BLEND));
			GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

			m_ShaderObject = std::make_unique<Shader>("src/res/Shaders/Stencil.shader");
			m_ShaderSingleColor = std::make_unique<Shader>("src/res/Shaders/StencilSingleColor.shader");
			m_VertexArrayCube = std::make_unique<VertexArray>();
			m_VertexbBuffer = std::make_unique<VertexBuffer>(cubeVertices, 6 * 30 * sizeof(float));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArrayCube->AddBuffer(*m_VertexbBuffer, layout);


			m_VertexArrayFloor = std::make_unique<VertexArray>();
			m_vertexBuffer2 = std::make_unique<VertexBuffer>(planeVertices, 2 * 15 * sizeof(float));
			///layout.push<float>(3);
			//layout.push<float>(2);
			m_VertexArrayFloor->AddBuffer(*m_vertexBuffer2, layout);

			m_texture1 = std::make_unique<Texture>("src/res/Picture/marble.jpg");
			m_texture2 = std::make_unique<Texture>("src/res/Picture/metal.png");

			m_ShaderObject->Bind();
			m_ShaderObject->SetUniform1i("texturel", 0);

		}


	}


	StencilTest::StencilTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateStencilTestVectors();
	}



	StencilTest::~StencilTest()
	{
	}

	glm::mat4 StencilTest::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void StencilTest::ProcessKeyboard(My_StencilTest_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == ST_FORWARD)
			Position += Front * velocity;
		if (direction == ST_BACKWARD)
			Position -= Front * velocity;
		if (direction == ST_LEFT)
			Position -= Right * velocity;
		if (direction == ST_RIGHT)
			Position += Right * velocity;
	}

	void StencilTest::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(ST_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(ST_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(ST_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(ST_RIGHT, deltaTime);
	}

	void StencilTest::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateStencilTestVectors();
	}

	void StencilTest::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void StencilTest::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void StencilTest::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		StencilTest* instance = static_cast<StencilTest*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void StencilTest::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void StencilTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void StencilTest::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		StencilTest* instance = static_cast<StencilTest*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void StencilTest::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void StencilTest::OnUpdate(float deltaTime)
	{

	}

	void StencilTest::OnRender()
	{

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Renderer renderer;

		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		
		


		//物体绘制 把物体设定为模板设置为1 
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));  
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

		m_ShaderObject->Bind();
		m_proj = glm::perspective(glm::radians(Zoom), 1.8f, 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_ShaderObject->SetUniformMat4f("projection", m_proj);
		m_ShaderObject->SetUniformMat4f("view", m_view);

		
		m_VertexArrayCube->Bind();
		//m_texture2->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_ShaderObject->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_ShaderObject->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		//绘制地板 并把地板也设置成1
		m_VertexArrayFloor->Bind();
		m_texture1->Bind();
		glm::mat4 model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, glm::vec3(0.0f, -1.0f, 0.0f));
		m_ShaderObject->SetUniformMat4f("model", model2);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		m_VertexArrayCube->UnBind();
		m_VertexArrayFloor->UnBind();


		//绘制物体轮廓 原理是放大物体本身 然后把刚刚设定为1的部分都不渲染 那么最终渲染出来的就是扩大那一点点
		GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));	
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
		GLCall(glDisable(GL_DEPTH_TEST));

		m_ShaderSingleColor->Bind();
		m_VertexArrayCube->Bind();
		m_texture2->Bind();
		float scale = 1.1f;


		m_ShaderSingleColor->SetUniformMat4f("projection", m_proj);
		m_ShaderSingleColor->SetUniformMat4f("view", m_view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		m_ShaderSingleColor->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		m_ShaderSingleColor->SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		GLCall(glEnable(GL_DEPTH_TEST));

	}

	void StencilTest::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void StencilTest::updateStencilTestVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void StencilTest::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& StencilTest::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}