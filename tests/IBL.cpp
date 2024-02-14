#include "IBL.h"


namespace test {
	IBL::IBL(GLFWwindow* window)
		:Position(glm::vec3(0.0f, 0.0f, 3.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(IBL_YAW), Pitch(IBL_PITCH), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(IBL_SPEED), MouseSensitivity(IBL_SENSITIVITY), Zoom(IBL_ZOOM), deltaTime(0.0f), lastFrame(0.0f), m_window(nullptr), CursorEnable(false), lastMouseX(0.0f), lastMouseY(0.0f), LightPos(2.0f, 4.0f, -2.0f), blinn(false), blinnKey(false)
	{
		{
			m_window = window;
			//Position = position;
			//WorldUp = up;
			//Yaw = yaw;
			//Pitch = pitch;
			updateIBLVectors();


			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glDepthFunc(GL_LEQUAL));
			
			//解决天空盒边缘过于明显
			GLCall(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
			



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

			//quad 
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};



			//Shader
			m_Shader = std::make_unique<Shader>("src/res/Shaders/IBL/PBR.shader");
			m_TransferCubeShader = std::make_unique<Shader>("src/res/Shaders/IBL/TransferToCubemap.shader");
			m_SkyLightShader = std::make_unique<Shader>("src/res/Shaders/IBL/CubeMapLightSampler.shader");
			m_backGroundShader = std::make_unique<Shader>("src/res/Shaders/IBL/BackGround.shader");
			m_preFilterShader = std::make_unique<Shader>("src/res/Shaders/IBL/prefilterShader.shader");
			m_brdfShader = std::make_unique<Shader>("src/res/Shaders/IBL/BRDF.shader");


			m_Shader->Bind();
			m_Shader->SetUniform1i("irradianceMap", 0);
			m_Shader->SetUniform1i("prefilterMap", 1);
			m_Shader->SetUniform1i("brdfLUT", 2);
			m_Shader->SetUniformVec3("albedo", 0.5f, 0.0f, 0.0f);
			m_Shader->SetUniform1f("ao", 1.0f);
			m_proj = glm::perspective(glm::radians(Zoom), (float)(960 / 540), 0.1f, 100.0f);
			m_Shader->SetUniformMat4f("projection", m_proj);



			m_backGroundShader->Bind();
			m_backGroundShader->SetUniform1i("environmentMap", 0);
			m_backGroundShader->SetUniformMat4f("projection", m_proj);

			//VAB VBO
			m_VertexArray = std::make_unique<VertexArray>();
			m_VertexBuffer = std::make_unique<VertexBuffer>(&m_data[0], m_data.size() * sizeof(float));
			m_IndexBuffer = std::make_unique<IndexBuffer>(&indices[0], indices.size());
			VertexBufferLayout layout;
			layout.push<float>(3);
			layout.push<float>(3);
			layout.push<float>(2);
			m_VertexArray->AddBuffer(*m_VertexBuffer, layout);


			//CUBE VAO VBO
			m_CubeVAO = std::make_unique<VertexArray>();
			m_CubeVBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
			VertexBufferLayout layoutCube;
			layoutCube.push<float>(3);
			layoutCube.push<float>(3);
			layoutCube.push<float>(2);
			m_CubeVAO->AddBuffer(*m_CubeVBO, layoutCube);


			//QuadVAO VBO
			m_QuadVAO = std::make_unique<VertexArray>();
			m_QuadVBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
			VertexBufferLayout layoutQuad;
			layoutQuad.push<float>(3);
			layoutQuad.push<float>(2);
			m_QuadVAO->AddBuffer(*m_QuadVBO, layoutQuad);

			//HDR
			std::string path = "src/res/Picture/HDR/studio.hdr";
			m_HDR_texture = std::make_unique<HDR_Load>(path);

			//帧缓冲
			m_IBL_FBO = std::make_unique<FBO_IBL_D>();
			

			//转换HDR到天空盒
			m_IBL_FBO->BindEnvCubeMap();
			for (unsigned int i = 0; i < 6; ++i)
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr));
			}

			m_TransferCubeShader->Bind();
			m_TransferCubeShader->SetUniform1i("equirectangularMap", 0);
			m_TransferCubeShader->SetUniformMat4f("projection", captureProjection);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_HDR_texture->HDRTextureNumber());

			GLCall(glViewport(0, 0, 512, 512));
			m_IBL_FBO->Bind();

			unsigned int tmpEnvCubeMap = m_IBL_FBO->envCubeMapNumber();
			for (unsigned int  i = 0; i < 6; ++i)
			{
				m_TransferCubeShader->SetUniformMat4f("view", captureViews[i]);
				GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL_FBO->envCubeMapNumber(), 0));
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
				m_CubeVAO->Bind();
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
				m_CubeVAO->UnBind();
			}
			m_IBL_FBO->UnBind();
			m_Shader->Unbind();

			//生成mipmap
			m_IBL_FBO->BindEnvCubeMap();
			//glBindTexture(GL_TEXTURE_CUBE_MAP, m_IBL_FBO->envCubeMapNumber());
			GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

			//采样天空和盒
			m_IBL_FBO->BindIrradianceMap();
			for (unsigned int  i = 0; i < 6; ++i)
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr));
			}

			m_IBL_FBO->Bind();
			m_IBL_FBO->Bindrbo();
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32));

			m_SkyLightShader->Bind();
			m_SkyLightShader->SetUniform1i("environmentMap", 0);
			m_SkyLightShader->SetUniformMat4f("projection", captureProjection);

			GLCall(glActiveTexture(GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IBL_FBO->envCubeMapNumber()));
			
			glViewport(0, 0, 32, 32);
			m_IBL_FBO->Bind();
			

			unsigned int tmpIrraadiance = m_IBL_FBO->irradianceMapNumber();
			for (unsigned int  i = 0; i < 6; ++i)
			{
				m_SkyLightShader->SetUniformMat4f("view", captureViews[i]);
				GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tmpIrraadiance, 0));
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
				m_CubeVAO->Bind();
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
				m_CubeVAO->UnBind();

			}
			m_IBL_FBO->UnBind();
			m_SkyLightShader->Unbind();

			//prefilterMAp
			m_IBL_FBO->BindPrefilterMap();
			unsigned int tmpPrefilterMap = m_IBL_FBO->prefilterMapNumber();
			for (unsigned int  i = 0; i < 6; ++i)
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr));
			}
			GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
			
			
			m_preFilterShader->Bind();
			m_preFilterShader->SetUniform1i("environmentMap", 0);
			m_preFilterShader->SetUniformMat4f("projection", captureProjection);
			GLCall(glActiveTexture(GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IBL_FBO->envCubeMapNumber()));
			
			m_IBL_FBO->Bind();


			unsigned int maxLevel = 5;
			for (unsigned int i = 0; i < maxLevel; ++i)
			{
				unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, i));
				unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, i));
				m_IBL_FBO->Bindrbo();
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
				glViewport(0, 0, mipWidth, mipHeight);
			
				float roughness = (float)i / (float)(maxLevel - 1);
				m_preFilterShader->SetUniform1f("roughness", roughness);
				for (unsigned int j = 0; j < 6; ++j)
				{
					m_preFilterShader->SetUniformMat4f("view", captureViews[j]);
					GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, m_IBL_FBO->prefilterMapNumber(), i))
			
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					m_CubeVAO->Bind();
					GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
					m_CubeVAO->UnBind();
				}
			}


			//unsigned int maxMipLevels = 5;
			//for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
			//{
			//	unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			//	unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			//	m_IBL_FBO->Bindrbo();
			//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			//	glViewport(0, 0, mipWidth, mipHeight);
			//
			//	float roughness = (float)mip / (float)(maxMipLevels - 1);
			//	m_preFilterShader->SetUniform1f("roughness", roughness);
			//	for (unsigned int i = 0; i < 6; ++i)
			//	{
			//		m_preFilterShader->SetUniformMat4f("view", captureViews[i]);
			//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tmpPrefilterMap, mip);
			//
			//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//		m_CubeVAO->Bind();
			//		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			//		m_CubeVAO->UnBind();
			//	}
			//}
			m_IBL_FBO->UnBind();

			
			//BRDF
			m_IBL_FBO->BindBRDF();
			unsigned int tmpBRDF = m_IBL_FBO->BRDFNumber();//不知道为什么这样不行
			
			//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0));

			m_IBL_FBO->Bind();
			m_IBL_FBO->Bindrbo();
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_IBL_FBO->BRDFNumber(), 0));

			glViewport(0, 0, 512, 512);
			m_brdfShader->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_QuadVAO->Bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			m_QuadVAO->UnBind();
			
			m_IBL_FBO->UnBind();
			glViewport(0, 0, 960, 540);
		}


	}


	IBL::IBL(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_window(nullptr)
	{
		m_window = window;
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateIBLVectors();
	}



	IBL::~IBL()
	{
	}

	glm::mat4 IBL::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void IBL::ProcessKeyboard(My_IBL_Movement direction, float deltaTime)
	{


		float velocity = MovementSpeed * deltaTime;
		if (direction == IBL_FORWARD)
			Position += Front * velocity;
		if (direction == IBL_BACKWARD)
			Position -= Front * velocity;
		if (direction == IBL_LEFT)
			Position -= Right * velocity;
		if (direction == IBL_RIGHT)
			Position += Right * velocity;
	}

	void IBL::ProcessKeyBoardInput(GLFWwindow* window, float deltaTime)
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
			this->ProcessKeyboard(IBL_FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->ProcessKeyboard(IBL_BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->ProcessKeyboard(IBL_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->ProcessKeyboard(IBL_RIGHT, deltaTime);

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

	void IBL::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
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
		updateIBLVectors();
	}

	void IBL::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

	void IBL::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		this->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void IBL::staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		IBL* instance = static_cast<IBL*>(glfwGetWindowUserPointer(window));
		instance->Scroll_callback(window, xoffset, yoffset);
	}

	void IBL::ActiveScrollCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, staticScrollCallback);
	}

	void IBL::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

	void IBL::staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		IBL* instance = static_cast<IBL*>(glfwGetWindowUserPointer(window));
		instance->mouse_callback(window, xpos, ypos);

	}

	void IBL::activeCursorPosCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, staticCursorPositionCallback);
	}


	float IBL::ourLerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	void IBL::OnUpdate(float deltaTime)
	{

	}

	void IBL::OnRender()
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

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IBL_FBO->irradianceMapNumber()));

		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IBL_FBO->prefilterMapNumber()));
		

		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_IBL_FBO->BRDFNumber()));

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

		m_backGroundShader->Bind();
		m_backGroundShader->SetUniformMat4f("view", m_view);
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_IBL_FBO->envCubeMapNumber()));
		//GLCall(glBindTexture(GL_TEXTURE_2D, m_IBL_FBO->BRDFNumber()));
		m_CubeVAO->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		m_CubeVAO->UnBind();

		//test BRDF
		//m_brdfShader->Bind();
		//m_QuadVAO->Bind();
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//m_QuadVAO->UnBind();

	}

	void IBL::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}


	void IBL::updateIBLVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	void IBL::setCursorMode(GLFWwindow* window, int cursorMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

		if (cursorMode == GLFW_CURSOR_DISABLED)
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		else if (cursorMode == GLFW_CURSOR_NORMAL)
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

	}

	Shader& IBL::GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr)
	{
		return *shaderPtr.get();
	}
}