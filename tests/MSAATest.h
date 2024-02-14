#pragma once
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Test.h"
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
#include "Shader.h"
#include "TestClass/FramebufferClass.h"
#include "TestClass//MASS_FrameBuffer.h"



#include <memory>


#include <vector>

enum MASSTest_Movement {
	MASS_FORWARD,
	MASS_BACKWARD,
	MASS_LEFT,
	MASS_RIGHT,
};

const float MASS_YAW = -90.0f;
const float MASS_PITCH = 0.0f;
const float MASS_SPEED = 5.5f;
const float MASS_SENSITIVITY = 0.1f;
const float MASS_ZOOM = 45.0f;


namespace test {
	class MASSTest : public Test
	{
	public:

		unsigned int N_Frame;
		unsigned int N_IFBO;

		unsigned int N_VAO1;
		unsigned int N_VAO2;

		GLFWwindow* m_window;

		int RockMeshSize;

		//光源位置
		glm::vec3 lightPos;


		//和光标出现消失相关的变量
		bool CursorEnable;
		double lastMouseX;
		double lastMouseY;

		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//代码移动尝试
		float matrixmove;

		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		//鼠标相关的变量
		bool FirstMouse = true; //是否是鼠标第一次接入
		int lastX, lastY;
		float deltaTime, lastFrame;

		MASSTest(GLFWwindow* window);
		MASSTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~MASSTest();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(MASSTest_Movement direction, float deltaTime);
		void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

		//鼠标滚轮缩放
		void ProcessMouseScroll(float yoffset);
		void ScrolMASS_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void ActiveScrollCallback(GLFWwindow* window);

		//鼠标移动功能
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
		void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
		static void staticCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		void activeCursorPosCallback(GLFWwindow* window);


		//尝试光标隐藏和出现
		void setCursorMode(GLFWwindow* window, int cursorMode);


		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateLightVectors();


		std::unique_ptr<Shader> m_ShaderObject;
		std::unique_ptr<Shader> m_ShaderScreen;
		
		std::unique_ptr<VertexArray> Object_VAO;
		std::unique_ptr<VertexBuffer> Object_VBO;

		std::unique_ptr<VertexArray> Screen_VAO;
		std::unique_ptr<VertexBuffer> Screen_VBO;

		std::unique_ptr<FrameBuffer> InterFBO;
		std::unique_ptr<MASS_FrameBuffer> MASS_FRAMEBUFFER;

		
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;








	};

}
