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



#include <memory>


#include <vector>

enum My_StencilTest_Movement {
	ST_FORWARD,
	ST_BACKWARD,
	ST_LEFT,
	ST_RIGHT,
};

const float ST_YAW = -90.0f;
const float ST_PITCH = 0.0f;
const float ST_SPEED = 2.5f;
const float ST_SENSITIVITY = 0.1f;
const float ST_ZOOM = 45.0f;



namespace test {
	class StencilTest : public Test
	{
	public:
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

		StencilTest(GLFWwindow* window);
		StencilTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~StencilTest();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(My_StencilTest_Movement direction, float deltaTime);
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



		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateStencilTestVectors();

		std::unique_ptr<Shader> m_ShaderObject;
		std::unique_ptr<Shader> m_ShaderSingleColor;
		std::unique_ptr<VertexArray> m_VertexArrayCube;
		std::unique_ptr<VertexArray> m_VertexArrayFloor;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer2;
		std::unique_ptr<Texture> m_texture1;
		std::unique_ptr<Texture> m_texture2;

		glm::mat4 m_proj, m_view;





	};
}
