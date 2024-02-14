#pragma once
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"
#include "Vendor/STB/stb_image.h"
#include "Vendor/imgui/imgui.h"
#include "Model.h"
#include "mesh.h"
#include "TestClass/G_buffer.h"
#include "TestClass/FBO_SSAO.h"
#include "TestClass/FBO_SSAO_Blur.h"
#include "TestClass/NoiseTexture.h"
#include "TestClass/FBO_IBL_D.h"
#include "TestClass/HDR_Load.h"
#include "IndexBuffer.h"
#include <random>
#include <math.h>





#include <memory>


#include <vector>

enum My_IBL_Movement {
	IBL_FORWARD,
	IBL_BACKWARD,
	IBL_LEFT,
	IBL_RIGHT,
};

const float IBL_YAW = -90.0f;
const float IBL_PITCH = 0.0f;
const float IBL_SPEED = 2.5f;
const float IBL_SENSITIVITY = 0.1f;
const float IBL_ZOOM = 45.0f;



namespace test {
	class IBL : public Test
	{
	public:
		GLFWwindow* m_window;

		glm::vec3 LightPos;
		bool blinn;
		bool blinnKey;

		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;


		std::vector<glm::vec3> IBL_kernel;
		std::vector<glm::vec3> IBL_noise;


		glm::vec3 lightColor = glm::vec3(0.2f, 0.2f, 0.7f);

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;

		int nrRows = 7;
		int nrColums = 7;
		float space = 2.5f;

		const float PI = 3.14159265359;

		std::vector<glm::vec3>positions;
		std::vector<glm::vec2>uv;
		std::vector<glm::vec3>normals;
		std::vector<unsigned int> indices;
		std::vector<float> m_data;
		unsigned int indiceCount;

		unsigned int xxx;

		glm::vec3 lightPositions[4] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
		};
		glm::vec3 lightColors[4] = {
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f)
		};

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		glm::mat4 captureViews[6] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		int SCR_WIDTH ;
		int SCR_HEIGHT;

		//测试
		glm::mat4* matrices;
		unsigned int amount;

		const unsigned int NR_LIGHT = 32;

		const float linear = 0.09f;
		const float quadratic = 0.032f;


		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		//鼠标相关的变量
		bool FirstMouse = true; //是否是鼠标第一次接入
		int lastX, lastY;
		float deltaTime, lastFrame;

		//光标出现消失
		double lastMouseX, lastMouseY;
		bool CursorEnable;

		IBL(GLFWwindow* window);
		IBL(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~IBL();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(My_IBL_Movement direction, float deltaTime);
		void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

		//鼠标滚轮缩放
		void ProcessMouseScroll(float yoffset);
		void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void ActiveScrollCallback(GLFWwindow* window);

		//鼠标移动功能
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
		void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		void activeCursorPosCallback(GLFWwindow* window);

		void setCursorMode(GLFWwindow* window, int cursorMode);

		Shader& GetShaderFromUniquePtr(std::unique_ptr<Shader, std::default_delete<Shader>>& shaderPtr);

		//让更多点聚集与片段 少数点远离的公式
		float ourLerp(float a, float b, float f);

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateIBLVectors();

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_TransferCubeShader;
		std::unique_ptr<Shader> m_SkyLightShader;
		std::unique_ptr<Shader> m_preFilterShader;
		std::unique_ptr<Shader> m_brdfShader;
		std::unique_ptr<Shader> m_backGroundShader;


		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_CubeVBO;

		std::unique_ptr<VertexArray> m_QuadVAO;
		std::unique_ptr<VertexBuffer> m_QuadVBO;

		std::unique_ptr<HDR_Load> m_HDR_texture;

		std::unique_ptr<FBO_IBL_D> m_IBL_FBO;

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_lightPosview;





	};
}