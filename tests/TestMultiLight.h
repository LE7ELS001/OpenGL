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


#include <memory>


#include <vector>

//使用glfwSetCursorPosCallback(window, mouse_callback);接受鼠标移动数据
//使用glfwsetSCrollcallback(window,scroll_callback); 接收滚轮输入

enum My_MultiLight_Movement {
	J_FORWARD,
	J_BACKWARD,
	J_LEFT,
	J_RIGHT,
};

const float J_YAW = -90.0f;
const float J_PITCH = 0.0f;
const float J_SPEED = 2.5f;
const float J_SENSITIVITY = 0.1f;
const float J_ZOOM = 45.0f;

//glm::vec3 cubePositions[] = {
//		glm::vec3(0.0f,  0.0f,  0.0f),
//		glm::vec3(2.0f,  5.0f, -15.0f),
//		glm::vec3(-1.5f, -2.2f, -2.5f),
//		glm::vec3(-3.8f, -2.0f, -12.3f),
//		glm::vec3(2.4f, -0.4f, -3.5f),
//		glm::vec3(-1.7f,  3.0f, -7.5f),
//		glm::vec3(1.3f, -2.0f, -2.5f),
//		glm::vec3(1.5f,  2.0f, -2.5f),
//		glm::vec3(1.5f,  0.2f, -1.5f),
//		glm::vec3(-1.3f,  1.0f, -1.5f)
//};


namespace test {
	class MultiLight : public Test
	{
	public:

		glm::vec3 cubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		glm::vec3 pointLightPosition[4] = {
			glm::vec3(0.7f,0.2f,2.0f),
			glm::vec3(2.3f,-3.3f,-4.0f),
			glm::vec3(-4.0f,2.0f,-12.0f),
			glm::vec3(0.0f,0.0f,-3.0f)
		};


		GLFWwindow* m_window;

		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

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

		MultiLight(GLFWwindow* window);
		MultiLight(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~MultiLight();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(My_MultiLight_Movement direction, float deltaTime);
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



		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateMultiLightVectors();

		std::unique_ptr<Shader> m_ShaderObject;
		std::unique_ptr<Shader> m_ShaderLightCube;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexArray> m_VertexArrayObject;
		std::unique_ptr<VertexArray> m_VertexArrayLightCube;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;

		glm::mat4 m_proj, m_view;





	};

}
