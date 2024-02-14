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
#include "TestClass/TextureGamma.h"




#include <memory>


#include <vector>

enum My_SteepParallaxMapping_Movement {
	SPM_FORWARD,
	SPM_BACKWARD,
	SPM_LEFT,
	SPM_RIGHT,
};

const float SPM_YAW = -90.0f;
const float SPM_PITCH = 0.0f;
const float SPM_SPEED = 2.5f;
const float SPM_SENSITIVITY = 0.1f;
const float SPM_ZOOM = 45.0f;



namespace test {
	class SteepParallaxMapping : public Test
	{
	public:
		glm::vec3 lightPositions[4] = {
		glm::vec3(-3.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
		};

		glm::vec3 lightColors[4] = {
			glm::vec3(0.25),
			glm::vec3(0.50),
			glm::vec3(0.75),
			glm::vec3(1.00)
		};

		float heightScale = 0.1f;


		GLFWwindow* m_window;

		glm::vec3 LightPos;
		bool gammaEnable;
		bool gammaKey;

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

		SteepParallaxMapping(GLFWwindow* window);
		SteepParallaxMapping(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~SteepParallaxMapping();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(My_SteepParallaxMapping_Movement direction, float deltaTime);
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
		void updateSteepParallaxMappingVectors();

		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;

		std::unique_ptr<Texture> m_texture;
		std::unique_ptr<Texture> m_textureNormal;
		std::unique_ptr<Texture> m_textureParallax;


		glm::mat4 m_proj, m_view, m_model;





	};
}