#pragma once
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
#include "Model.h"




#include <memory>


#include <vector>

enum GeometryTestTwo_Movement {
	GEOT_FORWARD,
	GEOT_BACKWARD,
	GEOT_LEFT,
	GEOT_RIGHT,
};

const float GEOT_YAW = -90.0f;
const float GEOT_PITCH = 0.0f;
const float GEOT_SPEED = 2.5f;
const float GEOT_SENSITIVITY = 0.1f;
const float GEOT_ZOOM = 45.0f;


namespace test {
	class GeometryTestTwo : public Test
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

		GeometryTestTwo(GLFWwindow* window);
		GeometryTestTwo(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~GeometryTestTwo();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(GeometryTestTwo_Movement direction, float deltaTime);
		void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

		//����������
		void ProcessMouseScroll(float yoffset);
		void ScrolGEOT_callback(GLFWwindow* window, double xoffset, double yoffset);
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


		std::unique_ptr<ShaderTwo> m_Shader;
		std::unique_ptr<Model> m_Model;

		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;






	};

}
