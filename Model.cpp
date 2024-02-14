#include"Model.h"

Model::Model( std::string const &path, bool gamma ) : gammaCorrection(false)
{
	gammaCorrection = gamma;
	loadModel(path);
}

void Model::Draw(Shader &shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Me_Draw(shader);
}

void Model::Draw(ShaderTwo& shaderTwo)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Me_Draw(shaderTwo);
}


void Model::loadModel(std::string const &path)
{
	Assimp::Importer importer;  // assimp���е�importerȥ����

	//���ȴ���һ��*scene������������aiScene) �����������ش������ϲ��  ʹ��importer��ReadFile����ȥ��ȡģ��
	//��һ����������·���� �ڶ���������һЩ���� ��һ����������������� ���漸������� 
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace); 
	
	//���¼�������ᱨ�� �������û������ �������ɲ���ȫ ����û�и��ڵ� ��ô��ӡ������Ϣ
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << "ASSIMP LOADED ERROR," << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/')); //�����·�� �����path��������

	processNode(scene->mRootNode, scene); //����ڵ�node
}

void Model::GetTexId(std::vector<Me_Texture> texture_load, std::string pathname)
{
	auto it = std::find_if(texture_load.begin(), texture_load.end(), [&](const Me_Texture& m) {return MathPathname(m, pathname); });
}

bool Model::MathPathname(const Me_Texture& m, const std::string& targetPathname)
{
	return(m.path == targetPathname);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	//����ȡ��mesh mesh��scene�������һ������  node->mnummeshes��������˼ ȡ��mesh�ĸ��� �Ӷ�֪����Ҫѭ�����ΰ�����Ū��
	//֮������node�����meshes ����Ϊnode���洢�����meshes������ ��Ϊһ��ģ�Ϳ��ܻ��в�ͬ��layer ���糵 �������� ���� ���ǿ���������mesh ��node�������ǵ�����
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; //scene���mesh�ĸ�ʽ��aiMesh ����ȡ��node�����mmeshes ����i��ȷ���ǵڼ��� 
		meshes.push_back(processMesh(mesh, scene));      //��ȡ�õ�meshѹ�������Լ���mesh���洢������
	}


	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


//����mesh mesh��������������� vertices , indices, textures ��һ������ֱ��һ��ȡ�� �п��ܻ��һ���ϵ ����indices��face�й� textures��material�й�
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Me_Vertext> vertices;
	std::vector<unsigned int> indices;
	std::vector<Me_Texture> textures;


	//vertices (vertices������� λ�� ���� �������� ���� ������
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//����λ�ü��ص��Լ���mesh
		Me_Vertext vertex1;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex1.Position = vector;


		//���߼��ص��Լ���mesh
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex1.Normal = vector;
		}


		//����������ص��Լ���mesh
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 TexC;  //ע������������vec2
			TexC.x = mesh->mTextureCoords[0][i].x; //��Ϊ������������к���8�� ����ֻ�õ���һ�� ������[0]
			TexC.y = mesh->mTextureCoords[0][i].y;
			vertex1.TexCoords = TexC;

			//tangent���ߵĴ��� 
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex1.Tangent = vector;

			//bitangent�����ߵĴ��� 

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex1.Bitangent = vector;
		}
		else
		{
			vertex1.TexCoords = glm::vec2(0.0f, 0.0f); //��һ������������ û�о��趨Ϊ0.0 0.0
		}
		vertices.push_back(vertex1);               //���ѵ�ǰ���vertexѹ��vector<Me_vertex> vertices��
	}

	
	

		// ȡ��indices ��������Ҫ�ҵ�һ���棨face) ��Ϊindex������������ mesh->mnumfaces��������˼ ȡ������ Ȼ�����
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];                       //face������������aiFace �ѵ�ǰi��face��ֵ��face
			for (unsigned int j = 0; j < face.mNumIndices; j++)	 //һ��face�����indices�п��ܲ�ֹһ�� ����������Ҫ���� ��face.mNumIndices��ȡ��indices�ĸ���
			{
				indices.push_back(face.mIndices[j]);
			}
		}


		// ȡ��texture ����Ҫ�ҵ�material ��Ϊtexture�в�ͬ������ ���� �߹�� ���Ƕ�������material��  mMaterials[mesh->mMaterialIndex]����Ϊһ��meshֻ����һ��material �������Ϳ���ֻ��һ��materialIndex�Ϳ��Ա���ȫ��
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];


		//�����Ǽ��˵� ��Ϊ������˵shaderҪд��sample2D �Ǵ�1����������Ⱦ��
		//���ֲ�ͬ��texture���ն�װ�����ǵ�textures�� �о�˳��Ӧ�úʹ���˳��һ�� 
		std::vector<Me_Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Me_Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Me_Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Me_Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		GetMeshId = mesh->mMaterialIndex; //�����ȡ��mesh��ID ����������model�� ����Ҫ����mesh

		return Mesh(vertices, indices, textures, GetMeshId);   //������� �Ϳ��Է���һ����������vertices, indices, textures��Mesh��
	
}

//����material��Ĳ�ͬ���� ����ΪAimaterial�����Ѿ����� aiTextureType�����assimp�涨�����ͣ� ������������� ���Ӧ�ú�shader���Լ�mesh������
std::vector<Me_Texture> Model::loadMaterialTextures(aiMaterial *materials, aiTextureType type, std::string typeName)
{
	std::vector<Me_Texture> textures;
	for (unsigned int i = 0; i < materials->GetTextureCount(type); i++)          //��ȡ�ض�������ĸ��� �������Ǵ����aitextureType��diffuse ��ô����ֻ����������ĸ��� specular�����ȡ 
	{																	         
		aiString str;													         
		materials->GetTexture(type, i, &str);							         //����ǻ�ȡ��[i]��ĳ���ض����͵����� Ȼ���������ΪaiString��str ���������ô������ʽ���� 
		bool skip = false;												         
		for (unsigned int j = 0; j < textures_loaded.size(); j++)                //�Ż� ������ǵ�ǰ׼������������Ƿ������֮ǰ��������һ���� һ���ľ��������� �����Ͳ����˷������ٴ���һ��
		{																         //ȡ��textures_loaded�Ĵ�С ȷ��Ҫ��������
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)	 //Ŀ��std::strcmp��compare�Ĺ��� �Ա��������� ��ͬ��¼�� ��ͬ��break����һ��ѭ��
			{
				textures.push_back(textures_loaded[j]);                          //�Ѿ����ڵĻ���Ͳ������½��� ��֮ǰ���ڵľ���
				skip == true;
				break;
			}
		}
		if (!skip)     //֮ǰû�е����
		{
			Me_Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);										//���ص��ҵ�textures���� ���·���
			textures_loaded.push_back(texture);									//��ӵ�textures_loaded���� ���¼���Ƿ����ظ�

		}
	}
		return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &Directory, bool gamma )  //�ѹ�Ҫ����Ƿ����ͬ���������Ż� ��Ϊ��������Ĳ��軹����
{
	std::string filename = std::string(path);   //�ļ��� ����model.obj
	filename = directory + '/' + filename;		//·��  c/file  ��ô������� c/file + / + model.obj 

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		
	}
	else
	{
		std::cout << "Failed to load Texture :" << path <<  std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


