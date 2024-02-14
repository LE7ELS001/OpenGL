#include "Post_process.h"

Post_process::Post_process(GameShader shader, unsigned int width, unsigned int height)
	:m_Shader(shader), m_Texture(), Width(width), Height(height), Confuse(false), Chaos(false), Shake(false)
{
	//好像是多重采样帧款冲
	glGenFramebuffers(1, &this->MSFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);

	//多重采样帧缓冲附件
	glGenRenderbuffers(1, &this->RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	//检查
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//普通帧款冲
	glGenFramebuffers(1, &this->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

	//普通帧款冲附件
	this->m_Texture.Generate(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_Texture.TextureID, 0);
	//检查
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER,0);


	//初启化
	this->initRenderData();
	this->m_Shader.SetInterger("scene", 0, true);
	
	
	//offset
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{-offset, offset},
		{-offset, offset},
		{ offset, offset},
		{-offset, 0.0f},
		{0.0f   , 0.0f},
		{offset , 0.0f},
		{-offset, -offset},
		{0.0f, -offset},
		{offset, -offset},
	};
	glUniform2fv(glGetUniformLocation(this->m_Shader.m_ShaderID, "offsets"), 9, (float*)offsets);

	
	//edge_kernel
	int edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->m_Shader.m_ShaderID, "edge_kernel"), 9, edge_kernel);

	//blur_kernel
	float blur_kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	glUniform1fv(glGetUniformLocation(this->m_Shader.m_ShaderID, "blur_kernel"), 9, blur_kernel);


}

void Post_process::BeginRender()
{
	//多重采样帧款冲绑定
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Post_process::EndRender()
{
	//多重采样帧款冲过渡到普通帧款冲
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Post_process::Render(float time)
{
	this->m_Shader.Use();
	this->m_Shader.SetFloat("time", time);
	this->m_Shader.SetInterger("confuse", Confuse);
	this->m_Shader.SetInterger("chaos", Chaos);
	this->m_Shader.SetInterger("shake", Shake);

	glActiveTexture(GL_TEXTURE0);
	this->m_Texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Post_process::initRenderData()
{
	unsigned int VBO;
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,	1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}
