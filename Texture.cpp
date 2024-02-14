#include "Renderer.h"
#include "Texture.h"
#include "Vendor/STB/stb_image.h"

Texture::Texture(const std::string& path) 
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), ID(0)
{
	//加载图片
	//stbi_set_flip_vertically_on_load(true);
	
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 加载图片（路径， 图片长宽像素储存在什么变量上， 预期通道 填4是因为RBGA）
	if (!m_LocalBuffer)
	{
		std::cout << "failed to load image" << std::endl;
	}
	//创建款冲
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); 

	//设置纹理参数
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	
	
	//给OPENGL传送数据
	//参数：1.纹理目标 2.多级渐变纹理等级 0为基本级别， 3.纹理储存的形式 RGBA8， 4和5 设置最终纹理的宽高， 6. 不知道 总是设为0 ， 7和8 定义了源图的格式和数据类型， 9真正的图像数据 你可以先不给 这样就是先开辟空间 之后怎么用之后再传
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA , GL_UNSIGNED_BYTE, m_LocalBuffer));
	
	//解绑
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	//检测缓冲里是否有东西 有就释放了 但是实际可能会保存下来
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


