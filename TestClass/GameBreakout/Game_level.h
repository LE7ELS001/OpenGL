#pragma once
#include "GL/glew.h"
#include "Vendor/glm/glm.hpp"

#include "GameBObject.h"
#include "Sprite_Render.h"
#include "Resource_manager.h"

#include <vector>
#include <sstream>
#include <fstream>

class Game_level
{
public :
	std::vector<GameBOject> Bricks; //用于存储不同的砖块数据

	Game_level() {}

	void Load(const char* file, unsigned int levelWdith, unsigned int levelHeight);

	void Draw(Sprite_Render& renderer);

	bool IsComplteted();

private :
	
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};
