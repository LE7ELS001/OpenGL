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

//��Ϸ״̬
enum GameBreakoutState {
	GAMEB_ACTIVE,
	GAMEB_MENU,
	GAMEB_WIN
};

//4��ש�������������ķ���
enum GameDirection
{
	GAME_UP,
	GAME_RIGHT,
	GAME_DOWN,
	GAME_LEFT
};

//��Ҳٿص�ƽ��λ�ô�С
const glm::vec2 PLAYER_SIZE(100, 20);
const float PLAYER_VELOCITY(500.0f);

//��Ĵ�С �� �ٶ�
const float BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

//����װ�����ײ���
typedef std::tuple<bool, GameDirection, glm::vec2> Collision;

class GameMain
{
public :
	GameBreakoutState State;
	bool Keys[1024];
	bool KeysProcessed[1024];
	unsigned int Width, Height;

	//����ؿ�
	std::vector<Game_level> Levels;
	unsigned int Level;

	//�������
	std::vector<Power_Up> PowerUps;

	//�������
	unsigned int Lives;

	GameMain(unsigned int width, unsigned int height);
	~GameMain();

	//��ʼ��
	void Init();

	
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	//========��ײ���===============
	void DoCollisions();
	GameDirection VectorDirection(glm::vec2 target);
	Collision CheckCollision(BallObject& one, GameBOject& two);
	bool CheckCollisionSimple(GameBOject& one, GameBOject& two);

	//========�������===============
	void SpawnPowerUps(GameBOject &block);
	void UpdatePowerUps(float dt);
	bool IsOtherPowerUpActive(std::vector<Power_Up>& powerups, std::string type);
	void ActivatePowerUp(Power_Up& powerUp);
	bool ShouldSpawn(unsigned int chance);

	//========���ùؿ������λ��=============
	//�ؿ�
	void ResetLevel();

	//���
	void ResetPlayer();


};
