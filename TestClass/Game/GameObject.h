#pragma once
#include "Renderer.h"
#include "SpriteRender.h"
#include "Vendor/glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "TestClass/Game/ResourceManager.h"
#include "TestClass/Game/BrickTexture.h"


/*
* 游戏物品类 控制砖块之类的
*/

class GameObject
{
public :
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	float Rotation;
	bool IsSolid;
	bool Destroyed;
	
	BrickTexture M_texture;
	

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	GameObject(BrickTexture sprtie,  glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(const std::unique_ptr<SpriteRender>& renderer, const std::unique_ptr<Shader>& shader, const std::unique_ptr<Texture>& texture);
	virtual void DrawBricks(const std::unique_ptr<SpriteRender>& renderer, const std::unique_ptr<Shader>& shader);

	
};