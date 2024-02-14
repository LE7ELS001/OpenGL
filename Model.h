#pragma once

#include <GL/glew.h>

#include <Vendor/glm/glm.hpp>
#include <Vendor/glm/gtc/matrix_transform.hpp>
#include <Vendor/STB/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<Shader.h>
#include<VertexArray.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Renderer.h>
#include<Texture.h>
#include <VertexBufferLayout.h>
#include <mesh.h>
#include <TestClass/ShaderTwo.h>


#include <string>
#include <vector>
#include<iostream>
#include <fstream>
#include <sstream>
#include <map>

class Model {
public:
	std::vector<Me_Texture> textures_loaded; // ����Ƿ�����Ƿ��Ѿ����� ���ⷴ������ ��getuniform location ����
	std::vector<Mesh> meshes; 
	std::string directory;
	unsigned int GetMeshId; // ��ΪmeshIDҪ��model������
	bool gammaCorrection; //٤��У��


	Model( std::string const &path, bool gamma);

	void Draw(Shader &shader);
	void Draw(ShaderTwo& shaderTwo);


private:
	void loadModel(std::string const&path);
	void GetTexId(std::vector<Me_Texture> texture_load, std::string pathname);
	bool MathPathname(const Me_Texture& m, const std::string& targetPathname);


	void processNode(aiNode*node , const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);


	std::vector<Me_Texture> loadMaterialTextures(aiMaterial *materials, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& Directory, bool gamma = false);

};	
