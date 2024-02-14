#include "Renderer.h"
#include "Texture.h"
#include "Vendor/STB/stb_image.h"

Texture::Texture(const std::string& path) 
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), ID(0)
{
	//����ͼƬ
	//stbi_set_flip_vertically_on_load(true);
	
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // ����ͼƬ��·���� ͼƬ�������ش�����ʲô�����ϣ� Ԥ��ͨ�� ��4����ΪRBGA��
	if (!m_LocalBuffer)
	{
		std::cout << "failed to load image" << std::endl;
	}
	//�������
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); 

	//�����������
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	
	
	//��OPENGL��������
	//������1.����Ŀ�� 2.�༶��������ȼ� 0Ϊ�������� 3.���������ʽ RGBA8�� 4��5 ������������Ŀ�ߣ� 6. ��֪�� ������Ϊ0 �� 7��8 ������Դͼ�ĸ�ʽ���������ͣ� 9������ͼ������ ������Ȳ��� ���������ȿ��ٿռ� ֮����ô��֮���ٴ�
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA , GL_UNSIGNED_BYTE, m_LocalBuffer));
	
	//���
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	//��⻺�����Ƿ��ж��� �о��ͷ��� ����ʵ�ʿ��ܻᱣ������
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

	ID = m_RendererID;
	
}

Texture::Texture()
{
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D,0));
}

unsigned int Texture::GetTextureID() const 
{
	return m_RendererID;
}


