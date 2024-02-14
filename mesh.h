#pragma once

#include <GL/glew.h>
#include <Vendor/glm/glm.hpp>
#include<Vendor/glm/gtc/matrix_transform.hpp>

#include<Shader.h>
#include<VertexArray.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Renderer.h>
#include<Texture.h>
#include <VertexBufferLayout.h>
#include "TestClass/ShaderTwo.h"


#include <string>
#include <vector>



#define MAX_BONE_INFLUENCE 4

//����
struct Me_Vertext {
	
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	//Tangent ���� Bitangent ������ ��һ�����������������������������ϵ�ļ�������
	glm::vec3 Tangent;
	glm::vec3 Bitangent;


	//������Ȩ��
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];

};


//����
struct Me_Texture {
	unsigned int ID;
	std::string type;
	std::string path;
};

//������

class Mesh {
public:
	std::vector<Me_Vertext> Me_vertices; //����
	std::vector<unsigned int> Me_indices; //����
	std::vector<Me_Texture> Me_texture;  //����
	unsigned int matID; //�����mesh���е�ID ���������ɫ������ID�����ض��Ĳ���
	unsigned int VAO;


	//������
	Mesh(std::vector<Me_Vertext> vertices, std::vector<unsigned int> indices, std::vector<Me_Texture> textures, unsigned int meshid);

	void Me_Draw(Shader &shader);
	void Me_Draw(ShaderTwo& shaderTwo);


private:
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IndiceBuffer;

	void setupMesh();

	unsigned int VBO, EBO;
};

