#pragma once
#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "TestClass/GameBreakout/GameTexture.h"
#include "Sprite_Render.h"

/*
	ÕÊº“œ‡πÿ
*/


class GameBOject
{
public: 
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	float Rotation;
	bool IsSolid;
	bool Destroyed;

	GameTexture m_Texture;

	GameBOject();
	GameBOject(glm::vec2 pos, glm::vec2 size, GameTexture texture, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(Sprite_Render& renderer);
};
