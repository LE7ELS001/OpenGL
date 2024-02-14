#include "ShadowDepth.h"

namespace test {
	ShadowDepth::ShadowDepth(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(SD_YAW), Pitch(SD_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SD_SPEED), MouseSensitivity(SD_SENSITIVITY), Zoom(SD_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), 
		 LightPos(0.0,0.0,0.0), LightProjection(glm::mat4(1.0f)), LightView(glm::mat4(1.0f)), LightSpaceMartrix(glm::mat4(1.0f)), ShadowON(false), ShadowKey(false)
	{
		{
			m_window = window;
			updateShadowDepthVectors();

			GLCall(glViewport(0, 0, 960, 540));

			GLCall(glEnable(GL_DEPTH_TEST));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小
			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//地板
			float planeVertices[] = {
				 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
				-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
				-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

				 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
				-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
				 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
			};

			//立方体
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				 // bottom face
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 // top face
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};

			//屏幕的正方形
			float quadVertices[] = {
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};





			//Shader
			m_DepthShader = std::make_unique<Shader>("src/res/Shaders/ShadowMap_Depth.shader");
			//m_QuadShader = std::make_unique<Shader>("src/res/Shaders/ShadowDepth_quad.shader");
			m_Shader = std::make_unique<Shader>("src/res/Shaders/ShadowMap.shader");
			m_Shader->Bind();
			m_Shader->SetUniform1i("diffuseTexture",0);
			m_Shader->SetUniform1i("shadowMap",1);

			//地板VAO VBO
			m_PlaneVertexArray = std::make_unique<VertexArray>();
			m_PlaneVertexbBuffer = std::make_unique<VertexBuffer>(&planeVertices, sizeof(planeVertices));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_PlaneVertexArray->AddBuffer(*m_PlaneVertexbBuffer, layout);

			//方块VAO VBO
			m_CubeVertexArray = std::make_unique<VertexArray>();
			m_CubeVertexBuffer = std::make_unique<VertexBuffer>(&vertices, sizeof(vertices));
			m_CubeVertexArray->AddBuffer(*m_CubeVertexBuffer, layout);

			//屏幕四边形VAO VBO
			m_QuadVertexArray = std::make_unique<VertexArray>();
			m_QuadVertexBuffer = std::make_unique<VertexBuffer>(&quadVertices, sizeof(quadVertices));
			VertexBufferLayout QuadLayout;
			QuadLayout.push<float>(3);
			QuadLayout.push<float>(2);
			m_QuadVertexArray->AddBuffer(*m_QuadVertexBuffer, QuadLayout);


			//地板材料
			m_Texture = std::make_unique<Texture>("src/res/Picture/wood.png");

			//深度帧缓冲
			m_DepthMapFBO = std::make_unique<DepthMapFBO>();

			//光源位置
			LightPos = glm::vec3(-2.0f, 4.0f, -1.0f);

			//获取由光源出发的投影矩阵的数据
			LightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			LightView = glm::lookAt(LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
			LightSpaceMartrix = LightProjection * LightView;


			glClearColor(0.1f, 0.1f, 0.1f, 0.1f);



		}


	}


	ShadowDepth::ShadowDepth(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateShadowDepthVectors();
	}



	ShadowDepth::~ShadowDepth()
	{
	}

	glm::mat4 ShadowDepth::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ShadowDepth::ProcessKeyboard(My_ShadowDepth_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == SD_FORWARD)
			Position += Front * velocity;
		if (direction == SD_BACKWARD)
			Position -= Front * velocity;
		if (direction == SD_LEFT)
			Position -= Right * velocity;
		if (direction == SD_RIGHT)
			Position += Right * velocity;
	}

	void ShadowDepth::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(SD_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(SD_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(SD_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(SD_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !ShadowKey)
		{
			ShadowON = !ShadowON;
			ShadowKey = true;
			std::cout << ShadowON << std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		{
			ShadowKey = false;
		}

		
	}

	void ShadowDepth::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateShadowDepthVectors();
	}

	void ShadowDepth::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void ShadowDepth::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void ShadowDepth::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ShadowDepth* instance = static_cast<ShadowDepth*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void ShadowDepth::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void ShadowDepth::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void ShadowDepth::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		ShadowDepth* instance = static_cast<ShadowDepth*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void ShadowDepth::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void ShadowDepth::OnUpdate(float deltaTime)
	{

	}

	void ShadowDepth::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		glfwPollEvents();

		//利用深度帧缓冲获取深度值
		m_DepthShader->Bind();
		m_DepthShader->SetUniformMat4f("lightSpaceMatrix", LightSpaceMartrix);
		
		GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
		
		m_DepthMapFBO->Bind();
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
		m_Texture->Bind();
		GLCall(glEnable(GL_CULL_FACE));
		GLCall(glCullFace(GL_FRONT));
		RenderScene(*m_DepthShader);
		GLCall(glCullFace(GL_BACK));
		GLCall(glDisable(GL_CULL_FACE));
		m_DepthMapFBO->UnBind();

		
		//正常绘制出场景
		GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		//GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色

		m_Shader->Bind();
		m_view = GetViewMatrix();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);

		m_Shader->SetUniformVec3("viewPos", Position);
		m_Shader->SetUniformVec3("lightPos", LightPos);
		m_Shader->SetUniformMat4f("lightSpaceMatrix", LightSpaceMartrix);
		m_Shader->SetUniform1i("shadows", ShadowON);

		//纹理
		m_Texture->Bind(0);
		m_DepthMapFBO->BindDepthMap(1);
		RenderScene(*m_Shader);

		//屏幕四边形
		//m_QuadShader->Bind();
		//m_QuadShader->SetUniform1f("near_plane", near_plane);
		//m_QuadShader->SetUniform1f("far_plane", far_plane);
		//m_DepthMapFBO->BindDepthMap(0);
		//m_QuadVertexArray->Bind();
		//GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		//m_QuadVertexArray->UnBind();
		//glfwSwapBuffers(m_window);
		//glfwPollEvents();
	}

	void ShadowDepth::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void ShadowDepth::updateShadowDepthVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	//分情况绘制 一种是为了得到深度的绘制 一种是最终输出画面的绘制 取决用什么shader
	void ShadowDepth::RenderScene(Shader& shader)
	{
		//地板绘制
		glm::mat4 model = glm::mat4(1.0f);
		shader.SetUniformMat4f("model", model);
		m_PlaneVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		m_PlaneVertexArray->UnBind();
		
		//方块绘制
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		shader.SetUniformMat4f("model", model);
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
		shader.SetUniformMat4f("model", model);
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
		model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		model = glm::scale(model,glm::vec3(0.5f));
		shader.SetUniformMat4f("model", model);
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

	}

	void ShadowDepth::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& ShadowDepth::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}