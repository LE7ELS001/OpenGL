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



#include <memory>


#include <vector>

enum InstanceTest_Movement {
	INST_FORWARD,
	INST_BACKWARD,
	INST_LEFT,
	INST_RIGHT,
};

const float INST_YAW = -90.0f;
const float INST_PITCH = 0.0f;
const float INST_SPEED = 30.5f;
const float INST_SENSITIVITY = 0.1f;
const float INST_ZOOM = 45.0f;


namespace test {
	class InstanceTest : public Test
	{
	public:
		GLFWwindow* m_window;

		int RockMeshSize;

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

		InstanceTest(GLFWwindow* window);
		InstanceTest(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~InstanceTest();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(InstanceTest_Movement direction, float deltaTime);
		void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

		//����������
		void ProcessMouseScroll(float yoffset);
		void ScrolINST_callback(GLFWwindow* window, double xoffset, double yoffset);
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


		std::unique_ptr<Shader> m_ShaderRock;
		std::unique_ptr<Shader> m_ShaderPlanet;
		std::unique_ptr<Shader> m_rocktestshader;

		std::unique_ptr<Model> m_ModelRock;
		std::unique_ptr<Model> m_ModelPlanet;

		std::unique_ptr<VertexBuffer> m_InstanceVBO;

		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;








	};

}
