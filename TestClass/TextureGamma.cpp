#include "TextureGamma.h"
#include "Renderer.h"
#include "Vendor/STB/stb_image.h"

TextureGamma::TextureGamma(const std::string& path, bool gammaCorrection)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), InternalFormat(GL_RED), DataFormat(GL_RED)
{

	

	////加载图片
	//stbi_set_flip_vertically_on_load(true);
	//m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0); // 加载图片（路径， 图片长宽像素储存在什么变量上， 预期通道 填4是因为RBGA）
	//if (m_LocalBuffer)
	//{
	//	//检查图片格式
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
	//		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;  //决定纹理到底是在线性空间还是在非线性空间中处理
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
	////创建款冲
	//GLCall(glGenTextures(1, &m_RendererID));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, m_Width, m_Height, 0, DataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//
	////设置纹理参数
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	////解绑
	//GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	//
	//
	////检测缓冲里是否有东西 有就释放了 但是实际可能会保存下来
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