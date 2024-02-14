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
#include "TestClass/DepthCubeMapFBO.h"




#include <memory>


#include <vector>

enum My_PointShadowDepth_Movement {
	PSD_FORWARD,
	PSD_BACKWARD,
	PSD_LEFT,
	PSD_RIGHT,
};

const float PSD_YAW = -90.0f;
const float PSD_PITCH = 0.0f;
const float PSD_SPEED = 2.5f;
const float PSD_SENSITIVITY = 0.1f;
const float PSD_ZOOM = 45.0f;



namespace test {
	class PointShadowDepth : public Test
	{
	public:
		GLFWwindow* m_window;

		const int SHADOW_WIDTH = 1024;
		const int SHADOW_HEIGHT = 1024;

		bool ShadowON;
		bool ShadowKey;

		glm::mat4 PointLightProj;
		std::vector<glm::mat4> ShadowTransforms;

		//光源位置
		glm::vec3 LightPos;

		//光源投射的近远面
		float near_plane = 1.0f;
		float far_plane = 25.0f;

		//光源头投影矩阵等
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

		//鼠标相关的变量
		bool FirstMouse = true; //是否是鼠标第一次接入
		int lastX, lastY;
		float deltaTime, lastFrame;

		//光标出现消失
		double lastMouseX, lastMouseY;
		bool CursorEnable;

		PointShadowDepth(GLFWwindow* window);
		PointShadowDepth(GLFWwindow* window, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~PointShadowDepth();


		glm::mat4 GetViewMatrix();

		//键盘移动
		void ProcessKeyboard(My_PointShadowDepth_Movement direction, float deltaTime);
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


		void RenderScene(Shader& shader);
		void RenderScene(ShaderTwo& shader);

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void updatePointShadowDepthVectors();

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<ShaderTwo> m_DepthShader;

		std::unique_ptr<VertexArray> m_CubeVertexArray;
		std::unique_ptr<VertexBuffer> m_CubeVertexBuffer;

		std::unique_ptr<Texture> m_Texture;

		std::unique_ptr<DepthCubeMapFBO> m_DepthMapFBO;



		glm::mat4 m_proj, m_view, m_model;





	};
}