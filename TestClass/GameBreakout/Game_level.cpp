#include "Game_level.h"

void Game_level::Load(const char* file, unsigned int levelWdith, unsigned int levelHeight)
{
	this->Bricks.clear(); //清理可能的旧数据

	unsigned int tileCode;
	Game_level level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;

	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);

		}
		if (tileData.size() > 0)
		{
			this->init(tileData, levelWdith, levelHeight);
		}
	}
}

void Game_level::Draw(Sprite_Render& renderer)
{
	for (GameBOject& tile : this->Bricks)
	{
		if (!tile.Destroyed)
			tile.Draw(renderer);
	}
}

bool Game_level::IsComplteted()
{
	for (GameBOject& tile : this->Bricks)
	{
		if (!tile.IsSolid && !tile.Destroyed)
		{
			return false;
		}
	}
	return true;
}

void Game_level::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	//计算每个瓦片的尺寸
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight /height;
	//float unit_height = levelHeight / static_cast<float>(height);

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (tileData[y][x] == 1) //实心砖块 
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameBOject obj(pos, size, Resource_manager::GetTexture("BlockSoild"), glm::vec3(0.8f, 0.8f, 0.7f)); //确保在执行前在resourceManager里添加了纹理
				obj.IsSolid = true;
				this->Bricks.push_back(obj); //基本信息都储存在obj里面 
			}
			else if (tileData[y][x] > 1) //普通砖块
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData[y][x] == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData[y][x] == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData[y][x] == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameBOject obj2(pos, size, Resource_manager::GetTexture("block"), color);
				this->Bricks.push_back(obj2);
			}
		}
		
	}
}
