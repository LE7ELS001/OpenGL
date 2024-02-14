#pragma once

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"

#include <vector>

//使用glfwSetCursorPosCallback(window, mouse_callback);接受鼠标移动数据
//使用glfwsetSCrollcallback(window,scroll_callback); 接收滚轮输入

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
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

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f), float yaw = YAW , float pitch = PITCH );
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();

	//键盘移动
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessKeyBoardInput(GLFWwindow* window, float deltaTime);

	//鼠标滚轮缩放
	void ProcessMouseScroll(float yoffset);
	void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	//鼠标移动功能
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
	

private:
	void updateCameraVectors();



};

