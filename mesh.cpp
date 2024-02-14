#include <mesh.h>

Mesh::Mesh(std::vector<Me_Vertext> vertices, std::vector<unsigned int> indices, std::vector<Me_Texture> textures, unsigned int meshid)
{
	
		this->Me_vertices = vertices;
		this->Me_indices = indices;
		this->Me_texture = textures;
		this->matID = meshid;
		setupMesh();
}

//绘制
void Mesh::Me_Draw(Shader& shader)
{
	
		//一个模型中 散射 高光 法线 凹凸？的数量不止一个 所以根据数量进行名字的改变
		unsigned int diffuseNr = 1; 
		unsigned int specularNr = 1;

		//
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;


		//Me_texture.size()是查询有几个纹理 然后根据数量设定循环次数 
		for (unsigned int i = 0; i < Me_texture.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));			//激活纹理单元 好像是因为纹理单元是一个int还是啥 所以可以直接+i 去取下一个纹理单元
			std::string name;
			std::string number;
			name = Me_texture[i].type;							//获取当前纹理的类型并储存到name当中
			if (name == "texture_diffuse")						//然后根据名字 分为不同的纹理 并对相应的纹理进行添加
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal")
			{
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height")
			{
				number = std::to_string(heightNr++);
			}
			//std::cout << name + number << std::endl; //根据打印知道specular最多为1 但是diffuse最多可能是2 我也不太懂为什么我除重复的容器里为什么还有重复 而有18张贴图 但是diffuse最多就2个是为什么
			shader.SetUniform1i((name + number).c_str(), i);	//因为上面的数字已经转成std::String 所以相加是两段string的内容合并 所以会出现texture_diffuse1,texture_diffuse2之类的
			GLCall(glBindTexture(GL_TEXTURE_2D, Me_texture[i].ID));
			


		}
		Renderer render;
		
		render.Draw(*m_VAO.get(), *m_IndiceBuffer.get(), shader);
		
		glActiveTexture(GL_TEXTURE0);
		
}

void Mesh::Me_Draw(ShaderTwo& shaderTwo)
{
	//一个模型中 散射 高光 法线 凹凸？的数量不止一个 所以根据数量进行名字的改变
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	//
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;


	//Me_texture.size()是查询有几个纹理 然后根据数量设定循环次数 
	for (unsigned int i = 0; i < Me_texture.size(); i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i));			//激活纹理单元 好像是因为纹理单元是一个int还是啥 所以可以直接+i 去取下一个纹理单元
		std::string name;
		std::string number;
		name = Me_texture[i].type;							//获取当前纹理的类型并储存到name当中
		if (name == "texture_diffuse")						//然后根据名字 分为不同的纹理 并对相应的纹理进行添加
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height")
		{
			number = std::to_string(heightNr++);
		}
		//std::cout << name + number << std::endl; //根据打印知道specular最多为1 但是diffuse最多可能是2 我也不太懂为什么我除重复的容器里为什么还有重复 而有18张贴图 但是diffuse最多就2个是为什么
		shaderTwo.SetUniform1i((name + number).c_str(), i);	//因为上面的数字已经转成std::String 所以相加是两段string的内容合并 所以会出现texture_diffuse1,texture_diffuse2之类的
		GLCall(glBindTexture(GL_TEXTURE_2D, Me_texture[i].ID));



	}
	Renderer render;

	render.Draw(*m_VAO.get(), *m_IndiceBuffer.get(), shaderTwo);

	glActiveTexture(GL_TEXTURE0);
}


//生成各种缓冲 
void Mesh::setupMesh()
{

	//VAO	
	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	//bufferdata(VAO)
	m_VertexBuffer = std::make_unique<VertexBuffer>(&Me_vertices[0], Me_vertices.size() * sizeof(Me_Vertext)); //注意sizeof里面的内容 之前就是写错了

	//EBO
	m_IndiceBuffer = std::make_unique<IndexBuffer>(&Me_indices[0], Me_indices.size());
	
	VertexBufferLayout layout;
	// 位置
	layout.push<float>(3);
	//法向
	layout.push<float>(3);
	//纹理坐标
	layout.push<float>(2);
	//Tangent
	layout.push<float>(3);
	//Bitangent
	layout.push<float>(3);
	//boneID
	layout.push<int>(4); //注意这个int的特殊类型 glVertexAttribIPointer(5, 4, GL_INT, sizeof(Me_Vertext), (void*)offsetof(Me_Vertext, m_BoneIDs));
	//Weight				首先 它用的是GLvertexAttribIpointer float用的是 glVertexAttribPointer 其次 他是没有转换类型哪个参数的  
	layout.push<float>(4);
	
	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	VAO = m_VAO->GetId();
	

}
