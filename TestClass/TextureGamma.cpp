#include "TextureGamma.h"
#include "Renderer.h"
#include "Vendor/STB/stb_image.h"

TextureGamma::TextureGamma(const std::string& path, bool gammaCorrection)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), InternalFormat(GL_RED), DataFormat(GL_RED)
{

	

	////����ͼƬ
	//stbi_set_flip_vertically_on_load(true);
	//m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0); // ����ͼƬ��·���� ͼƬ�������ش�����ʲô�����ϣ� Ԥ��ͨ�� ��4����ΪRBGA��
	//if (m_LocalBuffer)
	//{
	//	//���ͼƬ��ʽ
	//	GLenum internalFormat;
	//	GLenum dataFormat;
	//	if (m_BPP == 1)
	//	{
	//		internalFormat = dataFormat = GL_RED;
	//		InternalFormat = internalFormat;
	//		DataFormat = dataFormat;
	//	}
	//	else if (m_BPP == 3)
	//	{
	//		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;  //�����������������Կռ仹���ڷ����Կռ��д���
	//		dataFormat = GL_RGB;
	//		InternalFormat = internalFormat;
	//		DataFormat = dataFormat;
	//	}
	//	else if (m_BPP == 4)
	//	{
	//		internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
	//		dataFormat = GL_RGBA;
	//		InternalFormat = internalFormat;
	//		DataFormat = dataFormat;
	//	}	
	//}
	//else {
	//	std::cout << "Texture failed to load at path: " << path << std::endl;
	//}
	////�������
	//GLCall(glGenTextures(1, &m_RendererID));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, m_Width, m_Height, 0, DataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//
	////�����������
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	////���
	//GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	//
	//
	////��⻺�����Ƿ��ж��� �о��ͷ��� ����ʵ�ʿ��ܻᱣ������
	//if (m_LocalBuffer)
	//	stbi_image_free(m_LocalBuffer);

	{
			stbi_set_flip_vertically_on_load(true);
			glGenTextures(1, &m_RendererID);
			unsigned char* m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);
			if (m_LocalBuffer)
			{
				GLenum internalFormat;
				GLenum dataFormat;
				if (m_BPP == 1)
				{
					internalFormat = dataFormat = GL_RED;
				}
				else if (m_BPP == 3)
				{
					internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
					dataFormat = GL_RGB;
				}
				else if (m_BPP == 4)
				{
					internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
					dataFormat = GL_RGBA;
				}

				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(m_LocalBuffer);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(m_LocalBuffer);
			}

	}
	

}

TextureGamma::~TextureGamma()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void TextureGamma::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void TextureGamma::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}