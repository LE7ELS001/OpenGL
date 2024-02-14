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
#include "TestClass/DepthMapFBO.h"
#include "TestClass/TextureGamma.h"
#include "TestClass/Text.h"



#include <memory>


#include <vector>

enum My_Text_test_Movement {
	TT_FORWARD,
	TT_BACKWARD,
	TT_LEFT,
	TT_RIGHT,
};

const float TT_YAW = -90.0f;
const float TT_PITCH = 0.0f;
const float TT_SPEED = 2.5f;
const float TT_SENSITIVITY = 0.1f;
const float TT_ZOOM = 45.0f;



namespace test {
	class Text_test : public Test
	{
	public:
		GLFWwindow* m_window;

		const int SHADOW_WIDTH = 1024;
		const int SHADOW_HEIGHT = 1024;

		bool ShadowON;
		bool ShadowKey;

		//��Դλ��
		glm::vec3 LightPos;

		//��ԴͶ��Ľ�Զ��
		float near_plane = 1.0f;
		float far_plane = 7.5f;

		//��ԴͷͶӰ�����
		glm::mat4 LightProjection;
		glm::mat4 LightView;
		glm::mat4 LightSpaceMartrix;


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

		Text_test(GLFWwindow* window);
		Text_test(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~Text_test();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(My_Text_test_Movement direction, float deltaTime);
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


		void RenderScene(Shader& shader);

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateText_testVectors();

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_DepthShader;
		std::unique_ptr<Shader> m_QuadShader;

		std::unique_ptr<VertexArray> m_PlaneVertexArray;
		std::unique_ptr<VertexBuffer> m_PlaneVertexbBuffer;

		std::unique_ptr<VertexArray> m_CubeVertexArray;
		std::unique_ptr<VertexBuffer> m_CubeVertexBuffer;

		std::unique_ptr<VertexArray> m_QuadVertexArray;
		std::unique_ptr<VertexBuffer> m_QuadVertexBuffer;

		std::unique_ptr<Texture> m_Texture;

		std::unique_ptr<DepthMapFBO> m_DepthMapFBO;

		std::unique_ptr<Text> m_text;


		glm::mat4 m_proj, m_view;





	};
}