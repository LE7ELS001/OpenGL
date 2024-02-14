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

//顶点
struct Me_Vertext {
	
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	//Tangent 切线 Bitangent 副切线 是一种用于描述几何物体表面纹理坐标系的几何属性
	glm::vec3 Tangent;
	glm::vec3 Bitangent;


	//骨骼和权重
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];

};


//纹理
struct Me_Texture {
	unsigned int ID;
	std::string type;
	std::string path;
};

//网格类

class Mesh {
public:
	std::vector<Me_Vertext> Me_vertices; //顶点
	std::vector<unsigned int> Me_indices; //索引
	std::vector<Me_Texture> Me_texture;  //纹理
	unsigned int matID; //这个是mesh独有的ID 方便后面着色器根据ID来绑定特定的材质
	unsigned int VAO;


	//构造器
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

