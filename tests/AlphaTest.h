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

enum My_AlphaTest_Movement {
	AL_FORWARD,
	AL_BACKWARD,
	AL_LEFT,
	AL_RIGHT,
};

const float AL_YAW = -90.0f;
const float AL_PITCH = 0.0f;
const float AL_SPEED = 2.5f;
const float AL_SENSITIVITY = 0.1f;
const float AL_ZOOM = 45.0f;



namespace test {
	class AlphaTest : public Test
	{
	public:
		GLFWwindow* m_window;

		std::vector<glm::vec3> vegetation
		{
			glm::vec3(-1.5f, 0.0f, -0.48f),
			glm::vec3(1.5f, 0.0f, 0.51f),
			glm::vec3(0.0f, 0.0f, 0.7f),
			glm::vec3(-0.3f, 0.0f, -2.3f),
			glm::vec3(0.5f, 0.0f, -0.6f)
		};

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

		AlphaTest(GLFWwindow* window);
		AlphaTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~AlphaTest();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(My_AlphaTest_Movement direction, float deltaTime);
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
		void updateAlphaTestVectors();

		std::unique_ptr<Shader> m_ShaderObject;
		std::unique_ptr<VertexArray> m_VertexArrayCube;
		std::unique_ptr<VertexArray> m_VertexArrayFloor;
		std::unique_ptr<VertexArray> m_VertexArrayGrass;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer2;
		std::unique_ptr<VertexBuffer> m_vertexBuffer3;
		std::unique_ptr<Texture> m_texture1; //方块
		std::unique_ptr<Texture> m_texture2; //地板
		std::unique_ptr<Texture> m_texture3; //草

		glm::mat4 m_proj, m_view;





	};
}