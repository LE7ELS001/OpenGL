#include <mesh.h>

Mesh::Mesh(std::vector<Me_Vertext> vertices, std::vector<unsigned int> indices, std::vector<Me_Texture> textures, unsigned int meshid)
{
	
		this->Me_vertices = vertices;
		this->Me_indices = indices;
		this->Me_texture = textures;
		this->matID = meshid;
		setupMesh();
}

//����
void Mesh::Me_Draw(Shader& shader)
{
	
		//һ��ģ���� ɢ�� �߹� ���� ��͹����������ֹһ�� ���Ը��������������ֵĸı�
		unsigned int diffuseNr = 1; 
		unsigned int specularNr = 1;

		//
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;


		//Me_texture.size()�ǲ�ѯ�м������� Ȼ����������趨ѭ������ 
		for (unsigned int i = 0; i < Me_texture.size(); i++)
		{
			GLCall(glActiveTexture(GL_TEXTURE0 + i));			//��������Ԫ ��������Ϊ����Ԫ��һ��int����ɶ ���Կ���ֱ��+i ȥȡ��һ������Ԫ
			std::string name;
			std::string number;
			name = Me_texture[i].type;							//��ȡ��ǰ��������Ͳ����浽name����
			if (name == "texture_diffuse")						//Ȼ��������� ��Ϊ��ͬ������ ������Ӧ������������
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
			//std::cout << name + number << std::endl; //���ݴ�ӡ֪��specular���Ϊ1 ����diffuse��������2 ��Ҳ��̫��Ϊʲô�ҳ��ظ���������Ϊʲô�����ظ� ����18����ͼ ����diffuse����2����Ϊʲô
			shader.SetUniform1i((name + number).c_str(), i);	//��Ϊ����������Ѿ�ת��std::String �������������string�����ݺϲ� ���Ի����texture_diffuse1,texture_diffuse2֮���
			GLCall(glBindTexture(GL_TEXTURE_2D, Me_texture[i].ID));
			


		}
		Renderer render;
		
		render.Draw(*m_VAO.get(), *m_IndiceBuffer.get(), shader);
		
		glActiveTexture(GL_TEXTURE0);
		
}

void Mesh::Me_Draw(ShaderTwo& shaderTwo)
{
	//һ��ģ���� ɢ�� �߹� ���� ��͹����������ֹһ�� ���Ը��������������ֵĸı�
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	//
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;


	//Me_texture.size()�ǲ�ѯ�м������� Ȼ����������趨ѭ������ 
	for (unsigned int i = 0; i < Me_texture.size(); i++)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + i));			//��������Ԫ ��������Ϊ����Ԫ��һ��int����ɶ ���Կ���ֱ��+i ȥȡ��һ������Ԫ
		std::string name;
		std::string number;
		name = Me_texture[i].type;							//��ȡ��ǰ��������Ͳ����浽name����
		if (name == "texture_diffuse")						//Ȼ��������� ��Ϊ��ͬ������ ������Ӧ������������
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
		//std::cout << name + number << std::endl; //���ݴ�ӡ֪��specular���Ϊ1 ����diffuse��������2 ��Ҳ��̫��Ϊʲô�ҳ��ظ���������Ϊʲô�����ظ� ����18����ͼ ����diffuse����2����Ϊʲô
		shaderTwo.SetUniform1i((name + number).c_str(), i);	//��Ϊ����������Ѿ�ת��std::String �������������string�����ݺϲ� ���Ի����texture_diffuse1,texture_diffuse2֮���
		GLCall(glBindTexture(GL_TEXTURE_2D, Me_texture[i].ID));



	}
	Renderer render;

	render.Draw(*m_VAO.get(), *m_IndiceBuffer.get(), shaderTwo);

	glActiveTexture(GL_TEXTURE0);
}


//���ɸ��ֻ��� 
void Mesh::setupMesh()
{

	//VAO	
	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	//bufferdata(VAO)
	m_VertexBuffer = std::make_unique<VertexBuffer>(&Me_vertices[0], Me_vertices.size() * sizeof(Me_Vertext)); //ע��sizeof��������� ֮ǰ����д����

	//EBO
	m_IndiceBuffer = std::make_unique<IndexBuffer>(&Me_indices[0], Me_indices.size());
	
	VertexBufferLayout layout;
	// λ��
	layout.push<float>(3);
	//����
	layout.push<float>(3);
	//��������
	layout.push<float>(2);
	//Tangent
	layout.push<float>(3);
	//Bitangent
	layout.push<float>(3);
	//boneID
	layout.push<int>(4); //ע�����int���������� glVertexAttribIPointer(5, 4, GL_INT, sizeof(Me_Vertext), (void*)offsetof(Me_Vertext, m_BoneIDs));
	//Weight				���� ���õ���GLvertexAttribIpointer float�õ��� glVertexAttribPointer ��� ����û��ת�������ĸ�������  
	layout.push<float>(4);
	
	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	VAO = m_VAO->GetId();
	

}
