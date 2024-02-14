#include "HDR_Load.h"


HDR_Load::HDR_Load(const std::string& path)
	: m_RendererID(0), m_FilePath(path), data(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	//����ͼƬ
	stbi_set_flip_vertically_on_load(true);
	data = stbi_loadf(path.c_str(), &m_Width, &m_Height, &m_BPP, 0); // ����ͼƬ��·���� ͼƬ�������ش�����ʲô�����ϣ� Ԥ��ͨ�� ��4����ΪRBGA��
	if (!data)
	{
		std::cout << "failed to load image" << std::endl;
	}
	else
	{
		//�������
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, data));

		//�����������
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}
	
	//���
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//��⻺�����Ƿ��ж��� �о��ͷ��� ����ʵ�ʿ��ܻᱣ������
	if (data)
		stbi_image_free(data);



}

HDR_Load::~HDR_Load()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void HDR_Load::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE_2D + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void HDR_Load::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int HDR_Load::HDRTextureNumber()
{
	return m_RendererID;
}
