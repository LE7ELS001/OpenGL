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
//#include <iostream>




#include <memory>


#include <vector>

enum My_DeferredShading_Movement {
	DES_FORWARD,
	DES_BACKWARD,
	DES_LEFT,
	DES_RIGHT,
};

const float DES_YAW = -90.0f;
const float DES_PITCH = 0.0f;
const float DES_SPEED = 2.5f;
const float DES_SENSITIVITY = 0.1f;
const float DES_ZOOM = 45.0f;



namespace test {
	class DeferredShading : public Test
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

		std::vector<glm::vec3> objectPosition;
		std::vector<glm::vec3> LightPosition;
		std::vector<glm::vec3> LightColor;

		//测试
		glm::mat4* matrices;
		unsigned int amount;

		const unsigned int NR_LIGHT = 32;

		const float linear = 0.7f;
		const float quadratic = 1.8f;


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

		DeferredShading(GLFWwindow* window);
		DeferredShading(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~DeferredShading();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(My_DeferredShading_Movement direction, float deltaTime);
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
		void updateDeferredShadingVectors();

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_LightHandler;
		std::unique_ptr<Shader> m_LightBox;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexArray> m_VertexArrayQuad;
		std::unique_ptr<VertexBuffer> m_VertexBufferQuad;

		//测试
		std::unique_ptr<VertexArray> m_InstanceVAO;
		std::unique_ptr<VertexBuffer> m_InstanceVBO;

		std::unique_ptr<Model> m_backpack;

		std::unique_ptr<G_buffer> m_GBUffer;
		

		glm::mat4 m_proj, m_view, m_model;





	};
}