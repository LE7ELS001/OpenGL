#include "Point_Shadow_Depth.h"

namespace test {
	PointShadowDepth::PointShadowDepth(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(PSD_YAW), Pitch(PSD_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(PSD_SPEED), MouseSensitivity(PSD_SENSITIVITY), Zoom(PSD_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f),
		LightPos(0.0, 0.0, 0.0), LightProjection(glm::mat4(1.0f)), LightView(glm::mat4(1.0f)), LightSpaceMartrix(glm::mat4(1.0f)), ShadowON(false), ShadowKey(false), PointLightProj(glm::mat4(1.0f))
	{
		{
			m_window = window;
			updatePointShadowDepthVectors();

			GLCall(glViewport(0, 0, 960, 540));

			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glEnable(GL_CULL_FACE));
			//GLCall(glDepthFunc(GL_ALWAYS)); 可以设置深度怎么算 比如比设定的值大或者小
			//GLCall(glEnable(GL_BLEND));
			//GLCall(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

			




			//Shader
			m_Shader = std::make_unique<Shader>("src/res/Shaders/Point_Shader_depth/PointShader.shader");
			m_DepthShader = std::make_unique<ShaderTwo>("src/res/Shaders/Point_Shader_depth/PointShaderDepth.shader");
			
			m_Shader->Bind();
			m_Shader->SetUniform1i("diffuseTexture", 0);
			m_Shader->SetUniform1i("depthMap", 1);

			

			//方块VAO VBO
			m_CubeVertexArray = std::make_unique<VertexArray>();
			m_CubeVertexBuffer = std::make_unique<VertexBuffer>(&vertices, sizeof(vertices));
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_CubeVertexArray->AddBuffer(*m_CubeVertexBuffer, layout);


			//地板材料
			m_Texture = std::make_unique<Texture>("src/res/Picture/wood.png");

			//深度帧缓冲
			m_DepthMapFBO = std::make_unique<DepthCubeMapFBO>();

			//光源位置
			LightPos = glm::vec3(0.0f, 0.0f, 0.0f);

		}
	}


	PointShadowDepth::PointShadowDepth(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updatePointShadowDepthVectors();
	}



	PointShadowDepth::~PointShadowDepth()
	{
	}

	glm::mat4 PointShadowDepth::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void PointShadowDepth::ProcessKeyboard(My_PointShadowDepth_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == PSD_FORWARD)
			Position += Front * velocity;
		if (direction == PSD_BACKWARD)
			Position -= Front * velocity;
		if (direction == PSD_LEFT)
			Position -= Right * velocity;
		if (direction == PSD_RIGHT)
			Position += Right * velocity;
	}

	void PointShadowDepth::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(PSD_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(PSD_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(PSD_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(PSD_RIGHT, deltaTime);

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

	void PointShadowDepth::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updatePointShadowDepthVectors();
	}

	void PointShadowDepth::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void PointShadowDepth::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void PointShadowDepth::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		PointShadowDepth* instance = static_cast<PointShadowDepth*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void PointShadowDepth::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void PointShadowDepth::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void PointShadowDepth::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		PointShadowDepth* instance = static_cast<PointShadowDepth*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void PointShadowDepth::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	void PointShadowDepth::OnUpdate(float deltaTime)
	{

	}

	void PointShadowDepth::OnRender()
	{


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		activeCursorPosCallback(m_window);
		ActiveScrollCallback(m_window);
		ProcessKeyBoardInput(m_window, deltaTime);

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f)); //用于清理颜色缓冲区 并设定一种颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		LightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);


		//转换到六个面透视投影的矩阵
		PointLightProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		ShadowTransforms.push_back(PointLightProj * glm::lookAt(LightPos, LightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(PointLightProj * glm::lookAt(LightPos, LightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(PointLightProj * glm::lookAt(LightPos, LightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		ShadowTransforms.push_back(PointLightProj * glm::lookAt(LightPos, LightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		ShadowTransforms.push_back(PointLightProj * glm::lookAt(LightPos, LightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		ShadowTransforms.push_back(PointLightProj * glm::lookAt(LightPos, LightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));




		//利用深度帧缓冲获取深度值
		GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
		m_DepthMapFBO->Bind();
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
		m_DepthShader->Bind();
		for (unsigned int i = 0; i < 6; i++)
		{
			m_DepthShader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", ShadowTransforms[i]);
		}

		m_DepthShader->SetUniformVec3("lightPos", LightPos);
		m_DepthShader->SetUniform1f("far_plane", far_plane);
		RenderScene(*m_DepthShader);
		m_DepthMapFBO->UnBind();


		//正常绘制方块
		GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		m_Shader->Bind();
		m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
		m_view = GetViewMatrix();
		m_Shader->SetUniformMat4f("projection", m_proj);
		m_Shader->SetUniformMat4f("view", m_view);

		m_Shader->SetUniformVec3("viewPos", Position);
		m_Shader->SetUniformVec3("lightPos", LightPos);
		m_Shader->SetUniform1i("shadows", ShadowON);
		m_Shader->SetUniform1f("far_plane", far_plane);
		m_Texture->Bind();
		m_DepthMapFBO->BindDepthMap(1);
		RenderScene(*m_Shader);

		//纹理
		m_Texture->Bind(0);
		m_DepthMapFBO->BindDepthMap(1);
		RenderScene(*m_Shader);

	}

	void PointShadowDepth::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void PointShadowDepth::updatePointShadowDepthVectors()
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
	void PointShadowDepth::RenderScene(Shader& shader)
	{

		//方块绘制
		//环境方块
		m_model = glm::mat4(1.0f);
		m_model = glm::scale(m_model, glm::vec3(5.0f));
		shader.SetUniformMat4f("model", m_model);
		GLCall(glDisable(GL_CULL_FACE));
		shader.SetUniform1i("reverse_normals", 1);
		
		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		shader.SetUniform1i("reverse_normals", 0);
		GLCall(glEnable(GL_CULL_FACE));

		//小方块1
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(4.0f, -3.5f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块2
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-3.0f, -1.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块3
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(2.0f, 3.0f, 1.0f));
		m_model = glm::scale(m_model, glm::vec3(0.75f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块4
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-1.5f, 1.0f, 1.5f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块5
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-1.5f, 2.0f, -3.0f));
		m_model = glm::rotate(m_model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		m_model = glm::scale(m_model, glm::vec3(0.75f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

	}

	void PointShadowDepth::RenderScene(ShaderTwo& shader)
	{
		//方块绘制
		//环境方块
		m_model = glm::mat4(1.0f);
		m_model = glm::scale(m_model, glm::vec3(5.0f));
		shader.SetUniformMat4f("model", m_model);
		GLCall(glDisable(GL_CULL_FACE));

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		GLCall(glEnable(GL_CULL_FACE));

		//小方块1
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(4.0f, -3.5f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块2
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-3.0f, -1.0f, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块3
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(2.0f, 3.0f, 1.0f));
		m_model = glm::scale(m_model, glm::vec3(0.75f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块4
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-1.5f, 1.0f, 1.5f));
		m_model = glm::scale(m_model, glm::vec3(0.5f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();

		//小方块5
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(-1.5f, 2.0f, -3.0f));
		m_model = glm::rotate(m_model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		m_model = glm::scale(m_model, glm::vec3(0.75f));
		shader.SetUniformMat4f("model", m_model);

		m_CubeVertexArray->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVertexArray->UnBind();
	}

	void PointShadowDepth::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& PointShadowDepth::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}