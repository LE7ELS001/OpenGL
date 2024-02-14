#include "HDR_Load.h"


HDR_Load::HDR_Load(const std::string& path)
	: m_RendererID(0), m_FilePath(path), data(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	//加载图片
	stbi_set_flip_vertically_on_load(true);
	data = stbi_loadf(path.c_str(), &m_Width, &m_Height, &m_BPP, 0); // 加载图片（路径， 图片长宽像素储存在什么变量上， 预期通道 填4是因为RBGA）
	if (!data)
	{
		std::cout << "failed to load image" << std::endl;
	}
	else
	{
		//创建款冲
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, data));

		//设置纹理参数
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}
	
	//解绑
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//检测缓冲里是否有东西 有就释放了 但是实际可能会保存下来
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
