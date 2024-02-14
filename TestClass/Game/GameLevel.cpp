#include "GameLevel.h"




void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	this->Bricks.clear();

	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;



	if (fstat)
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
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}


//�����draw��ש���draw
void GameLevel::Draw(const std::unique_ptr<SpriteRender>& renderer, const std::unique_ptr<Shader> &shader)
{
	for (GameObject& tile : this->Bricks)
	{
		if (!tile.Destroyed)
		{
			tile.DrawBricks(renderer, shader);
		}
	}
}

bool GameLevel::IsCompleted()
{
	for (GameObject& tile : this->Bricks)
	{
		if (!tile.IsSolid && !tile.Destroyed)
			return false;
	}
	return false;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	//�����С����
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (tileData[y][x] == 1) //��ζ����ʵ��ש��
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(ResourceManger::GetTexture("block"), pos, size, glm::vec3(0.8f, 0.8f, 0.7f)); //������������ж�ֱ�Ӹ�GameObject�����������ǰ���ú�
				obj.IsSolid = true;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if(tileData[y][x] == 3)
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
				GameObject obj( ResourceManger::GetTexture("block_soild"), pos, size, color);
				this->Bricks.push_back(obj);
			}
		}
	}
}
