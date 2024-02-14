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
#include "TestClass/G_buffer.h"
#include "TestClass/FBO_SSAO.h"
#include "TestClass/FBO_SSAO_Blur.h"
#include "TestClass/NoiseTexture.h"
#include <random>





#include <memory>


#include <vector>

enum My_SSAO_Movement {
	SSAO_FORWARD,
	SSAO_BACKWARD,
	SSAO_LEFT,
	SSAO_RIGHT,
};

const float SSAO_YAW = -90.0f;
const float SSAO_PITCH = 0.0f;
const float SSAO_SPEED = 2.5f;
const float SSAO_SENSITIVITY = 0.1f;
const float SSAO_ZOOM = 45.0f;



namespace test {
	class SSAO : public Test
	{
	public:
		GLFWwindow* m_window;

		glm::vec3 LightPos;
		bool blinn;
		bool blinnKey;

		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		
		std::vector<glm::vec3> SSAO_kernel;
		std::vector<glm::vec3> SSAO_noise;

		//glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, -2.0f);
		glm::vec3 lightColor = glm::vec3(0.2f, 0.2f, 0.7f);

		//����
		glm::mat4* matrices;
		unsigned int amount;

		const unsigned int NR_LIGHT = 32;

		const float linear = 0.09f;
		const float quadratic = 0.032f;


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

		SSAO(GLFWwindow* window);
		SSAO(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~SSAO();


		glm::mat4 GetViewMatrix();

		//�����ƶ�
		void ProcessKeyboard(My_SSAO_Movement direction, float deltaTime);
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

		//�ø����ۼ���Ƭ�� ������Զ��Ĺ�ʽ
		float ourLerp(float a, float b, float f);

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updateSSAOVectors();

		std::unique_ptr<Shader> m_ShaderGeometry;
		std::unique_ptr<Shader> m_LightShader;
		std::unique_ptr<Shader> m_SSAOshader;
		std::unique_ptr<Shader> m_SSAOBlurShader;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexArray> m_VertexArrayQuad;
		std::unique_ptr<VertexBuffer> m_VertexBufferQuad;

		

		std::unique_ptr<Model> m_backpack;

		std::unique_ptr<G_buffer> m_GBUffer;

		std::unique_ptr<FBO_SSAO> m_SSAO;

		std::unique_ptr<FBO_SSAO_Blur> m_SSAO_Blur;

		std::unique_ptr<NoiseTexture> m_NoiseTexutre;

		

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_lightPosview;





	};
}
