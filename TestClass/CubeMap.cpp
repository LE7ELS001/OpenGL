#include "CubeMap.h"


CubeMap::CubeMap(const std::string& face1, const std::string& face2, const std::string& face3, const std::string& face4, const std::string& face5, const std::string& face6) : width(0),height(0),nrChannels(0),data(nullptr)
{
	faces = { face1,face2,face3,face4,face5,face6 };
	this->loadCubeMap(faces);
}

CubeMap::~CubeMap()
{
}

void CubeMap::loadCubeMap(std::vector<std::string> faces)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	stbi_set_flip_vertically_on_load(false);
	for (int i = 0; i < faces.size(); i++)
	{
		data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			//	顺序好像是 右 左 上 下 前 后

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to loaded CubeBox, the wrong faces is : " << faces[i] << std::endl;
			stbi_image_free(data);
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}


void CubeMap::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void CubeMap::UnBind()
{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}