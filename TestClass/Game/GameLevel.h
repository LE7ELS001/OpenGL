#pragma once
#include "Renderer.h"
#include "Vendor/glm/glm.hpp"

#include "SpriteRender.h"
#include "GameObject.h"

#include <vector>
#include <fstream>
#include <sstream>

/*
	¹Ø¿¨²¼ÖÃµÈ
*/

class GameLevel
{
public:
	std::vector<GameObject> Bricks;


	

	GameLevel() {};
	

	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

	void Draw(const std::unique_ptr<SpriteRender> &renderer, const std::unique_ptr<Shader>& shader);

	bool IsCompleted();

private:
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

	
};