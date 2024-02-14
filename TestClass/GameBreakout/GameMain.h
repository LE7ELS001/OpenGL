#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/irrklang/includes/irrKlang.h"


#include "TestClass/GameBreakout/Game_level.h"
#include "TestClass/GameBreakout/Resource_manager.h"
#include "TestClass/GameBreakout/Sprite_Render.h"
#include "TestClass/GameBreakout/BallObject.h"
#include "TestClass/GameBreakout/Game_Particle.h"
#include "TestClass/GameBreakout/Post_process.h"
#include "TestClass/GameBreakout/Power_Up.h"
#include "TestClass/GameBreakout/GameText.h"


#include <vector>
#include <tuple>

using namespace irrklang;

//游戏状态
enum GameBreakoutState {
	GAMEB_ACTIVE,
	GAMEB_MENU,
	GAMEB_WIN
};

//4个砖块和球可能碰到的方向
enum GameDirection
{
	GAME_UP,
	GAME_RIGHT,
	GAME_DOWN,
	GAME_LEFT
};

//玩家操控的平板位置大小
const glm::vec2 PLAYER_SIZE(100, 20);
const float PLAYER_VELOCITY(500.0f);

//球的大小 和 速度
const float BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

//用于装检测碰撞结果
typedef std::tuple<bool, GameDirection, glm::vec2> Collision;

class GameMain
{
public :
	GameBreakoutState State;
	bool Keys[1024];
	bool KeysProcessed[1024];
	unsigned int Width, Height;

	//储存关卡
	std::vector<Game_level> Levels;
	unsigned int Level;

	//储存道具
	std::vector<Power_Up> PowerUps;

	//玩家生命
	unsigned int Lives;

	GameMain(unsigned int width, unsigned int height);
	~GameMain();

	//初始化
	void Init();

	
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	//========碰撞相关===============
	void DoCollisions();
	GameDirection VectorDirection(glm::vec2 target);
	Collision CheckCollision(BallObject& one, GameBOject& two);
	bool CheckCollisionSimple(GameBOject& one, GameBOject& two);

	//========道具相关===============
	void SpawnPowerUps(GameBOject &block);
	void UpdatePowerUps(float dt);
	bool IsOtherPowerUpActive(std::vector<Power_Up>& powerups, std::string type);
	void ActivatePowerUp(Power_Up& powerUp);
	bool ShouldSpawn(unsigned int chance);

	//========重置关卡和玩家位置=============
	//关卡
	void ResetLevel();

	//玩家
	void ResetPlayer();


};
