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

enum Light_Movement {
	L_FORWARD,
	L_BACKWARD,
	L_LEFT,
	L_RIGHT,
};

const float L_YAW = -90.0f;
const float L_PITCH = 0.0f;
const float L_SPEED = 2.5f;
const float L_SENSITIVITY = 0.1f;
const float L_ZOOM = 45.0f;

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
	class Light : public Test
	{
	public:
		GLFWwindow* m_window;

		//��Դλ��
		glm::vec3 lightPos;


		//�͹�������ʧ��صı���
		bool CursorEnable;
		double lastMouseX;
		double lastMouseY;

		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//�����ƶ�����
		float matrixmove;

		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		//�����صı���
		bool FirstMouse = true; //�Ƿ�������һ�ν���
		int lastX, lastY;
		float deltaTime, lastFrame;

		Light(GLFWwindow* window);
		Light(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~Light();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(Light_Movement direction, float deltaTime);
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


		//���Թ�����غͳ���
		void setCursorMode(GLFWwindow* window, int cursorMode);


		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateLightVectors();

		std::unique_ptr<Shader> m_ShaderLightCube;
		std::unique_ptr<Shader> m_ShaderObject;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexArray> m_VertexArrayLightCube;
		std::unique_ptr<VertexArray> m_VertexArrayObject;
		std::unique_ptr<IndexBuffer> m_IndiceBuffer;
		std::unique_ptr<Texture> m_diffuseMap;
		std::unique_ptr<Texture> m_specularMap;
		std::unique_ptr<Texture> m_emissionMap;


		glm::mat4 m_proj, m_view;





	};

}
