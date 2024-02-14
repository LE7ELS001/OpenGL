#include "Game_Particle.h"

Game_Particle::Game_Particle(GameShader shader, GameTexture texture, unsigned int amount)
	: m_shader(shader), m_texture(texture), Amount(amount)
{
	this->Init();
}

void Game_Particle::Update(float dt, GameBOject& object, unsigned int newParticles, glm::vec2 offset)
{
	for (unsigned int i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->ResetParticle(this->particles[unusedParticle], object, offset);
	}

	for (unsigned int i = 0; i < this->Amount; ++i)
	{
		Particle& p = this->particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5f;
		}
	}
}

void Game_Particle::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->m_shader.Use();

	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->m_shader.SetVector2f("offset", particle.Position);
			this->m_shader.SetVector4f("color", particle.Color);
			this->m_texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game_Particle::Init()
{
	unsigned int VBO;

	float particle_quad[] = {
		0.0f,1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,

		0.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		1.0f,0.0f,1.0f,0.0f
	};

	//VAO,VBO����
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < this->Amount; ++i)
	{
		this->particles.push_back(Particle());
	}
}


unsigned int lastUsedParticle = 0;
unsigned int Game_Particle::firstUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < this->Amount; ++i)
	{
		if (this->particles[i].Life <= 0.0f) //���������������С�ڵ���0 ֤���Ѿ���ʧ�� ���Ը���
		{
			lastUsedParticle = i;	
			return i;
		}
	}

	for (unsigned int i = 0; i < lastUsedParticle; ++i) //��������һ��������ʧ�������ܻ�����һ������ֵ���ұ� 
	{
		if (this->particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0; //���������˷���û���������ڽ��������� �ǿ�����������������̫����

	return 0;
}

void Game_Particle::ResetParticle(Particle& particle, GameBOject& object, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}
