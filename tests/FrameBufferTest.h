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
#include "TestClass/FramebufferClass.h"



#include <memory>


#include <vector>

enum My_FrameBufferTest_Movement {
	FB_FORWARD,
	FB_BACKWARD,
	FB_LEFT,
	FB_RIGHT,
};

const float FB_YAW = -90.0f;
const float FB_PITCH = 0.0f;
const float FB_SPEED = 2.5f;
const float FB_SENSITIVITY = 0.1f;
const float FB_ZOOM = 45.0f;



namespace test {
	class FrameBufferTest : public Test
	{
	public:
		GLFWwindow* m_window;

		unsigned int Fbuffer;
		unsigned int Tbuffer;
		unsigned int Rbuffer;


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

		//�����صı���
		bool FirstMouse = true; //�Ƿ�������һ�ν���
		int lastX, lastY;
		float deltaTime, lastFrame;

		//��������ʧ
		double lastMouseX, lastMouseY;
		bool CursorEnable;

		FrameBufferTest(GLFWwindow* window);
		FrameBufferTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~FrameBufferTest();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(My_FrameBufferTest_Movement direction, float deltaTime);
		void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

		//����������
		void ProcessMouseScroll(float yoffset);
		void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void staticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void ActiveScrollCallback(GLFWwindow* window);

		//����ƶ�����
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
		void updateFrameBufferTestVectors();

		std::unique_ptr<Shader> m_ShaderObject;
		std::unique_ptr<Shader> m_ShaderScreen;
		std::unique_ptr<VertexArray> m_VertexArrayCube;
		std::unique_ptr<VertexArray> m_VertexArrayFloor;
		std::unique_ptr<VertexArray> m_VertexArraySC;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer2;
		std::unique_ptr<VertexBuffer> m_vertexBuffer3;
		std::unique_ptr<Texture> m_texture1; 
		std::unique_ptr<Texture> m_texture2; 
		std::unique_ptr<FrameBuffer> m_framebuffer;


		glm::mat4 m_proj, m_view;





	};
}
