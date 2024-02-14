#include "TestClass/GameBreakout/GameMain.h"

Sprite_Render* Renderer;
GameBOject* Player;
BallObject* Ball;
Game_Particle* Particles;
Post_process* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();
GameText* Text;

//球碰到实心砖震动时间
float ShakeTime = 0.0f;

GameMain::GameMain(unsigned int width, unsigned int height)
	: State(GAMEB_MENU), Keys(),KeysProcessed(), Width(width), Height(height), Level(0), Lives(5)
{
}

GameMain::~GameMain()
{
	SoundEngine->stopAllSounds();
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
	
	
}

void GameMain::Init()
{
	//往ResourceManager里加载需要的Shader和Texture
	//加载Shader====================================
	Resource_manager::LoadShader("src/res/Shaders/Game/sprite.shader", "sprite");
	Resource_manager::LoadShader("src/res/Shaders/Game/Particle.shader", "particle");
	Resource_manager::LoadShader("src/res/Shaders/Game/PostProcessing.shader", "PostProcessing");

	//设置Shader参数
	//Sprite
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	Resource_manager::GetShader("sprite").Use().SetInterger("sprite", 0);
	Resource_manager::GetShader("sprite").SetMatrix4("projection", projection);

	//Particle
	Resource_manager::GetShader("particle").Use().SetInterger("sprite", 0);
	Resource_manager::GetShader("particle").SetMatrix4("projection", projection);


	//Texture====================================
	Resource_manager::LoadTexture("src/res/Picture/Game/background.jpg", false,"background");
	Resource_manager::LoadTexture("src/res/Picture/Game/block.png", false, "block");
	Resource_manager::LoadTexture("src/res/Picture/Game/block_solid.png", false, "BlockSoild");
	Resource_manager::LoadTexture("src/res/Picture/Game/paddle.png", true, "paddle");
	Resource_manager::LoadTexture("src/res/Picture/funny2.PNG", true, "face");
	Resource_manager::LoadTexture("src/res/Picture/Game/particle.png", true, "particle");
	Resource_manager::LoadTexture("src/res/Picture/Game/powerup_chaos.png", true, "powerup_chaos");
	Resource_manager::LoadTexture("src/res/Picture/Game/powerup_confuse.png", true, "powerup_confuse");
	Resource_manager::LoadTexture("src/res/Picture/Game/powerup_increase.png", true, "powerup_increase");
	Resource_manager::LoadTexture("src/res/Picture/Game/powerup_passthrough.png", true, "powerup_passthrough");
	Resource_manager::LoadTexture("src/res/Picture/Game/powerup_speed.png", true, "powerup_speed");
	Resource_manager::LoadTexture("src/res/Picture/Game/powerup_sticky.png", true, "powerup_sticky");



	//================渲染器=====================
	GameShader tmp = Resource_manager::GetShader("sprite");
	Renderer = new Sprite_Render(tmp);

	//================粒子效果====================
	Particles = new Game_Particle(Resource_manager::GetShader("particle"), Resource_manager::GetTexture("particle"), 500);


	//================后期效果======================
	Effects = new Post_process(Resource_manager::GetShader("PostProcessing"), this->Width, this->Height);

	//=================文字=========================
	Text = new GameText(this->Width, this->Height);
	Text->Load("src/res/Font/OCRAEXT.TTF", 24);


	//===============导入关卡======================
	Game_level one;
	one.Load("src/res/Levels/one.lvl", this->Width, this->Height /2);

	Game_level two;
	two.Load("src/res/Levels/two.lvl", this->Width, this->Height /2);

	Game_level three;
	three.Load("src/res/Levels/three.lvl", this->Width, this->Height /2);

	Game_level four;
	four.Load("src/res/Levels/four.lvl", this->Width, this->Height /2);

	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	//===============玩家操控的平板=================
	glm::vec2 PlayerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameBOject(PlayerPos, PLAYER_SIZE, Resource_manager::GetTexture("paddle"));


	//===============球==========================
	glm::vec2 ballPos = PlayerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, Resource_manager::GetTexture("face"));

	//===============音频========================
	SoundEngine->play2D("src/res/Audio/BreakoutCut.mp3", true);
}

void GameMain::ProcessInput(float dt)
{
	//选关
	if (this->State == GAMEB_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAMEB_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = true;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
			{
				--this->Level;
			}	
			else
			{
				this->Level = 3;
			}
			this->KeysProcessed[GLFW_KEY_S] = true;
		}
	}

	//胜利
	if (this->State == GAMEB_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
			Effects->Chaos = false;
			this->State = GAMEB_MENU;
		}
	}


	if (this->State == GAMEB_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
				{
					Ball->Position.x -= velocity;
				}
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
				{
					Ball->Position.x += velocity;
				}
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
		{
			Ball->Stuck = false;
		}
		if (this->Keys[GLFW_KEY_P])
		{
			Player->Size += 50.0f;
		}
	}
}

void GameMain::Update(float dt)
{
	//球的移动
	Ball->Move(dt, this->Width);

	//检测碰撞
	this->DoCollisions();

	//设定震动结束
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
		{
			Effects->Shake = false;
		}
	}

	//更新粒子
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

	//更新道具状态
	this->UpdatePowerUps(dt);

	//检查球是否出界 以及玩家命还有多少
	if (Ball->Position.y >= this->Height)
	{
		--this->Lives;

		//判断命是否为0 决定重置关卡还是继续让玩家游玩
		if (this->Lives <= 0)
		{
			this->ResetLevel();
			this->State = GAMEB_MENU;
		}

		//不为0就重置玩家 让他继续试
		this->ResetPlayer();
	}

	//检查玩家是否打完砖块 
	if (this->State == GAMEB_ACTIVE && this->Levels[this->Level].IsComplteted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = true;
		this->State = GAMEB_WIN;
	}


}

void GameMain::Render()
{
	if (this->State == GAMEB_ACTIVE || this->State == GAMEB_MENU || this->State == GAMEB_WIN)
	{
		//多重采样帧款冲
		Effects->BeginRender();

		GameTexture tmp = Resource_manager::GetTexture("background");
		Renderer->DrawSprtie(tmp, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);

		for (Power_Up& powerUp : this->PowerUps)
		{
			if (!powerUp.Destroyed)
			{
				powerUp.Draw(*Renderer);
			}
		}

		Particles->Draw();
		Ball->Draw(*Renderer);

		//普通FBO
		Effects->EndRender();

		//默认帧缓冲（绘制到屏幕）	
		Effects->Render(glfwGetTime());

		//显示几条命
		std::stringstream ss; 
		ss << this->Lives;
		Text->RenderText("Lives: " + ss.str(), 5.0f, 5.0f, 1.0f);
	}

	if (this->State == GAMEB_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
	}

	if (this->State == GAMEB_WIN)
	{
		Text->RenderText("Congratulations, You WON~~", 320.f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
}

void GameMain::DoCollisions()
{
	//========================球和砖块的碰撞========================
	for (GameBOject& box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision))
			{
				if (!box.IsSolid)
				{
					//设置不再渲染
					box.Destroyed = true;

					//道具生成
					this->SpawnPowerUps(box);

					//音频
					SoundEngine->play2D("src/res/Audio/HitBlock.mp3", false);
				}
				else
				{
					//后期效果
					ShakeTime = 0.02f;
					Effects->Shake = true;
					
					//音频
					SoundEngine->play2D("src/res/Audio/HitSolid.wav", false);

				}

				GameDirection dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				if (!(Ball->PassThrough && !box.IsSolid)) //就算激活了 pass_through道具 也不能穿过实心砖块
				{
					if (dir == GAME_LEFT || dir == GAME_RIGHT)
					{
						Ball->Velocity.x = -Ball->Velocity.x;

						float Penetration = Ball->Radius - std::abs(diff_vector.x);
						if (dir == GAME_LEFT)
							Ball->Position.x += Penetration;
						else
						{
							Ball->Position.x -= Penetration;
						}
					}
					else
					{
						Ball->Velocity.y = -Ball->Velocity.y;

						float penetration = Ball->Radius - std::abs(diff_vector.y);
						if (dir == GAME_UP)
						{
							Ball->Position.y -= penetration;
						}
						else
						{
							Ball->Position.y += penetration;
						}
					}
				}
			}
		}

		//检查道具 情况1.道具掉落到边界 没接住 情况2.玩家的平台接住了
		for (Power_Up& powerup : this->PowerUps)
		{
			if (!powerup.Destroyed)
			{
				if (powerup.Position.y >= this->Height) //碰到窗口下方了
				{
					powerup.Destroyed = true;
				}

				if (CheckCollisionSimple(*Player, powerup)) //碰到玩家的平台了
				{
					ActivatePowerUp(powerup);
					powerup.Destroyed = true;
					powerup.Activated = true;
					SoundEngine->play2D("src/res/Audio/powerup.wav", false);

				}
			}
		}
	}


	//=============球和玩家平台的碰撞========================
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
		float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		float percentage = distance / (Player->Size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity; //好像是为了确保速度大小不会改

		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;

		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);

		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);

		//后期效果
		//让球粘在平台上 如果道具生效的话
		Ball->Stuck = Ball->Sticky;

		//音频
		SoundEngine->play2D("src/res/Audio/PlatformHit.wav", false);
	}
}

GameDirection GameMain::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] =
	{
		glm::vec2(0.0f,1.0f), //UP
		glm::vec2(1.0f,0.0f), //RIGHT	
		glm::vec2(0.0f,-1.0f), //DOWN
		glm::vec2(-1.0f,0.0f) //LEFT

	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return (GameDirection)best_match;
}

Collision GameMain::CheckCollision(BallObject& one, GameBOject& two)
{
	glm::vec2 center(one.Position + one.Radius); //获取球的中心

	//获取正方体的一半长的x,y 和中心点
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);

	//猜是为了找到P
	glm::vec2 difference = center - aabb_center;

	//clamp有点没懂
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	if (glm::length(difference) < one.Radius)
	{
		return std::make_tuple(true, VectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(false, GAME_UP, glm::vec2(0.0f, 0.0f));
	}
}

bool GameMain::CheckCollisionSimple(GameBOject& one, GameBOject& two)
{
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;

	return collisionX && collisionY;


}


void GameMain::SpawnPowerUps(GameBOject& block)
{
	//高概率生成负面道具 低概率生成正面道具
	if (ShouldSpawn(75)) 
	{
		this->PowerUps.push_back(Power_Up("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, Resource_manager::GetTexture("powerup_speed")));
	}
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(Power_Up("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, Resource_manager::GetTexture("powerup_sticky")));
	}
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(Power_Up("pass_through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, Resource_manager::GetTexture("powerup_passthrough")));
	}
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(Power_Up("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.Position, Resource_manager::GetTexture("powerup_increase")));
	}
	if (ShouldSpawn(15))
	{
		this->PowerUps.push_back(Power_Up("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 5.0f, block.Position, Resource_manager::GetTexture("powerup_confuse")));
	}
	if (ShouldSpawn(15))
	{
		this->PowerUps.push_back(Power_Up("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 5.0f, block.Position, Resource_manager::GetTexture("powerup_chaos")));
	}
}

//更新道具状态 不然无限持续
void GameMain::UpdatePowerUps(float dt)
{
	for (Power_Up& powerup : this->PowerUps)
	{
		powerup.Position += powerup.Velocity * dt; //道具下落
		if (powerup.Activated)
		{
			powerup.Duration -= dt; //持续时间不断减少

			if (powerup.Duration <= 0.0f)
			{
				powerup.Activated = false;

				if (powerup.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{
						Ball->Sticky = false;
						Player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerup.Type == "pass_through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass_through"))
					{
						Ball->PassThrough = false;
						Ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerup.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = false;
					}
				}
				else if (powerup.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effects->Chaos = false;
					}
				}
			}
		}
	}
	//把已经没用的道具 从std::vector<Power_up>移除 代码没太懂
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const Power_Up& powerup) {return powerup.Destroyed && !powerup.Activated; })
		, this->PowerUps.end());
}

//检查同类道具是否处于激活
bool GameMain::IsOtherPowerUpActive(std::vector<Power_Up>& powerups, std::string type)
{
	for (const Power_Up& powerup : powerups)
	{
		if (powerup.Activated)
		{
			if (powerup.Type == type)
				return true;
		}
	}
	return false;
}
//激活道具(赋予效果)
void GameMain::ActivatePowerUp(Power_Up& powerUp)
{
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2; 
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = true;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass_through")
	{
		Ball->PassThrough = true;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
		{
			Effects->Confuse = true;
		}
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Chaos)
		{
			Effects->Chaos = true;
		}
	}
}

//概率生成道具
bool GameMain::ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() & chance;
	return random == 0;
}

void GameMain::ResetLevel()
{
	if (this->Level == 0)
	{
		this->Levels[0].Load("src/res/Levels/one.lvl", this->Width, this->Height / 2);
	}
	else if(this->Level == 1)
	{
		this->Levels[1].Load("src/res/Levels/two.lvl", this->Width, this->Height / 2);
	}
	else if (this->Level == 2)
	{
		this->Levels[2].Load("src/res/Levels/three.lvl", this->Width, this->Height / 2);
	}
	else if (this->Level == 3)
	{
		this->Levels[3].Load("src/res/Levels/four.lvl", this->Width, this->Height / 2);
	}

	//后期会统计生命值
}

void GameMain::ResetPlayer()
{
	//主要是重置球和玩家的平台

	//玩家
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);

	//球
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

	//后期的特效 道具等
}
