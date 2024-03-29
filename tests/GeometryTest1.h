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
#include "TestClass/ShaderTwo.h"



#include <memory>


#include <vector>

enum GeometryTestOne_Movement {
	GEO_FORWARD,
	GEO_BACKWARD,
	GEO_LEFT,
	GEO_RIGHT,
};

const float GEO_YAW = -90.0f;
const float GEO_PITCH = 0.0f;
const float GEO_SPEED = 2.5f;
const float GEO_SENSITIVITY = 0.1f;
const float GEO_ZOOM = 45.0f;


namespace test {
	class GeometryTestOne : public Test
	{
	public:
		GLFWwindow* m_window;

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

		GeometryTestOne(GLFWwindow* window);
		GeometryTestOne(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~GeometryTestOne();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(GeometryTestOne_Movement direction, float deltaTime);
		void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

		//鼠标滚轮缩放
		void ProcessMouseScroll(float yoffset);
		void ScrolGEO_callback(GLFWwindow* window, double xoffset, double yoffset);
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

		std::unique_ptr<VertexArray>m_VertexArray;
		std::unique_ptr<VertexBuffer>m_VertexBuffer;
		//std::unique_ptr<ShaderTwo> m_Shader;
		std::unique_ptr<ShaderTwo> m_Shader;








	};

}
