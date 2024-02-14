#include "GameB.h"

/*
	��������ResourceManager���ڴ˴���ʼ��
*/

Game::Game(int width, int height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height), m_Renderer(), Levels(), Level(0)
{
}

Game::~Game()
{
	
}

void Game::Init()
{
	//=========================��ʼ��======================
	//=============Shader===================
	m_SpriteRenderShader = std::make_unique<Shader>("src/res/Shaders/Game/sprite.shader");
	
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
	m_SpriteRenderShader->Bind();
	m_SpriteRenderShader->SetUniform1i("sprite", 0);
	m_SpriteRenderShader->SetUniformMat4f("projection", projection);
	
	//=============Texture====================
	stbi_set_flip_vertically_on_load(true);
	m_FaceTexture = std::make_unique<Texture>("src/res/Picture/awesomeface.png");
	m_BackGround = std::make_unique<Texture>("src/res/Picture/Game/background.jpg");
	m_Paddle = std::make_unique<Texture>("src/res/Picture/Game/paddle.png");

	
	//========================��Ⱦ��========================
	m_Renderer = std::make_unique<SpriteRender>();

	//========================�ؿ�(��4�أ�==========================
	//����ؿ��ļ�
	GameLevel one;  
	one.Load("src/res/Levels/one.lvl", this->Width, this->Height * 0.5f);

	GameLevel two;
	two.Load("src/res/Levels/two.lvl", this->Width, this->Height * 0.5f);

	GameLevel three;
	two.Load("src/res/Levels/three.lvl", this->Width, this->Height * 0.5f);

	GameLevel four;
	two.Load("src/res/Levels/four.lvl", this->Width, this->Height * 0.5f);

	//���ص�Game�������Levels
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	//======================��ҿ��Ƶ����Ӳ���====================
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = std::make_unique<GameObject>(ResourceManger::GetTexture("paddle"), playerPos, PLAYER_SIZE);
	
	
}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;

		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x - +velocity;
			}
			if (this->Keys[GLFW_KEY_D])
			{
				if (Player->Position.x <= this->Width - Player->Size.x)
				{
					Player->Position.x += velocity;
				}
			}
		}
	}
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		//����
		m_Renderer->DrawSprite(m_SpriteRenderShader, m_BackGround, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
	}
	//�ؿ�
	this->Levels[this->Level].Draw(m_Renderer, m_SpriteRenderShader);

	//ƽ̨
	Player->DrawBricks(m_Renderer, m_SpriteRenderShader);

}
