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
	Assimp::Importer importer;  // assimp带有的importer去载入

	//首先创建一个*scene（它的类型是aiScene) 他是整个加载处于最上层的  使用importer的ReadFile函数去读取模型
	//第一个参数就是路径了 第二个参数是一些功能 第一个是网格呈现三角形 后面几个不清楚 
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace); 
	
	//以下几种情况会报错 如果场景没有生成 或者生成不完全 或者没有根节点 那么打印错误信息
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << "ASSIMP LOADED ERROR," << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/')); //这个是路径 后面的path才是名称

	processNode(scene->mRootNode, scene); //处理节点node
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
	//首先取得mesh mesh是scene里的其中一个数据  node->mnummeshes如字面意思 取得mesh的个数 从而知道我要循环几次把他们弄完
	//之所以用node里面的meshes 是因为node里面储存的是meshes的索引 因为一个模型可能会有不同的layer 比如车 会有轮子 车身 他们可能是两个mesh 而node会有他们的索引
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; //scene里的mesh的格式是aiMesh 这里取的node里面的mmeshes 根据i来确定是第几个 
		meshes.push_back(processMesh(mesh, scene));      //把取得的mesh压进我们自己的mesh里面储存起来
	}


	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


//处理mesh mesh里面会有以下数据 vertices , indices, textures 不一定都是直接一层取到 有可能会多一层关系 比如indices和face有关 textures和material有关
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Me_Vertext> vertices;
	std::vector<unsigned int> indices;
	std::vector<Me_Texture> textures;


	//vertices (vertices里面会有 位置 法线 纹理坐标 切线 副切线
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//顶点位置加载到自己的mesh
		Me_Vertext vertex1;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex1.Position = vector;


		//法线加载到自己的mesh
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex1.Normal = vector;
		}


		//纹理坐标加载到自己的mesh
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 TexC;  //注意纹理坐标是vec2
			TexC.x = mesh->mTextureCoords[0][i].x; //因为纹理坐标最多有好像8组 现在只用到第一组 所以是[0]
			TexC.y = mesh->mTextureCoords[0][i].y;
			vertex1.TexCoords = TexC;

			//tangent切线的传递 
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex1.Tangent = vector;

			//bitangent副切线的传递 

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex1.Bitangent = vector;
		}
		else
		{
			vertex1.TexCoords = glm::vec2(0.0f, 0.0f); //不一定有纹理坐标 没有就设定为0.0 0.0
		}
		vertices.push_back(vertex1);               //最后把当前组的vertex压进vector<Me_vertex> vertices里
	}

	
	

		// 取得indices 我们首先要找到一个面（face) 因为index的数据在面上 mesh->mnumfaces如字面意思 取得面数 然后遍历
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];                       //face的数据类型是aiFace 把当前i的face赋值到face
			for (unsigned int j = 0; j < face.mNumIndices; j++)	 //一个face里面的indices有可能不止一个 所以依旧需要便利 用face.mNumIndices来取得indices的个数
			{
				indices.push_back(face.mIndices[j]);
			}
		}


		// 取得texture 首先要找到material 因为texture有不同的类型 漫射 高光等 他们都储存在material里  mMaterials[mesh->mMaterialIndex]是因为一个mesh只会有一种material 所以他就可以只用一个materialIndex就可以遍历全部
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];


		//这里是简化了的 因为正常来说shader要写的sample2D 是从1到你的最大渲染数
		//四种不同的texture最终都装到我们的textures里 感觉顺序应该和处理顺序一致 
		std::vector<Me_Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Me_Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Me_Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Me_Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		GetMeshId = mesh->mMaterialIndex; //这里获取了mesh的ID 但是现在在model里 所以要传到mesh

		return Mesh(vertices, indices, textures, GetMeshId);   //处理完后 就可以返回一个带有所有vertices, indices, textures的Mesh了
	
}

//加载material里的不同纹理 参数为Aimaterial上面已经生成 aiTextureType这个是assimp规定的类型， 最后是类型名字 这个应该和shader里以及mesh里的相关
std::vector<Me_Texture> Model::loadMaterialTextures(aiMaterial *materials, aiTextureType type, std::string typeName)
{
	std::vector<Me_Texture> textures;
	for (unsigned int i = 0; i < materials->GetTextureCount(type); i++)          //获取特定的纹理的个数 比如我们传入的aitextureType是diffuse 那么他就只获得这个纹理的个数 specular不会获取 
	{																	         
		aiString str;													         
		materials->GetTexture(type, i, &str);							         //这个是获取第[i]个某个特定类型的纹理 然后给到类型为aiString的str 具体会是怎么样的形式忘了 
		bool skip = false;												         
		for (unsigned int j = 0; j < textures_loaded.size(); j++)                //优化 检测我们当前准备处理的纹理是否和我们之前的纹理是一样的 一样的就跳过处理 这样就不用浪费性能再处理一次
		{																         //取得textures_loaded的大小 确定要遍历几次
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)	 //目测std::strcmp是compare的功能 对比两个内容 不同就录入 相同就break掉这一次循环
			{
				textures.push_back(textures_loaded[j]);                          //已经存在的话你就不用重新建了 用之前存在的就行
				skip == true;
				break;
			}
		}
		if (!skip)     //之前没有的情况
		{
			Me_Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);										//加载到我的textures里面 等下返回
			textures_loaded.push_back(texture);									//添加到textures_loaded里面 等下检测是否有重复

		}
	}
		return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &Directory, bool gamma )  //难怪要检测是否存在同样纹理来优化 因为这个函数的步骤还不少
{
	std::string filename = std::string(path);   //文件名 比如model.obj
	filename = directory + '/' + filename;		//路径  c/file  那么这个就是 c/file + / + model.obj 

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


