#pragma once
#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "SpriteRender.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Vendor/STB/stb_image.h"

#include "TestClass/Game/GameLevel.h"
#include "TestClass/Game/GameObject.h"
#include "TestClass/Game/ResourceManager.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//==============玩家相关设置=================
const glm::vec2 PLAYER_SIZE(100, 20);
const float PLAYER_VELOCITY(500.0f);

class Game
{
public:
	GameState State;
	bool Keys[1024];
	int Width, Height;
	std::vector<GameLevel> Levels;
	unsigned int Level;


	//构造器
	Game(int width, int height);
	~Game();

	//初始化
	void Init();

	//游戏内循环
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

private:
	//Shader相关==================================
	std::unique_ptr<Shader> m_SpriteRenderShader;

	//Texture相关=================================
	std::unique_ptr<Texture> m_FaceTexture;
	std::unique_ptr<Texture> m_BackGround;
	std::unique_ptr<Texture> m_Paddle;


	//SpriteRender相关===========================
	std::unique_ptr<SpriteRender> m_Renderer;

	//GameObject相关==============================
	std::unique_ptr<GameObject> Player;

};


