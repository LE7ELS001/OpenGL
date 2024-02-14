#include "Text.h"

Text::Text() : m_Shader("src/res/Shaders/Text/Text.shader"), VAO(0), VBO(0), m_texture(0), m_character({ 0,{0,0},{0,0},0 })
{
	//Shader相关
	m_pro = glm::ortho(0.0f, static_cast<float>(Width), 0.0f, static_cast<float>(Height));
	m_Shader.Bind();
	m_Shader.SetUniformMat4f("projection", m_pro);

	//FreeType相关
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Error::FreeType: Could not init FreeType Library" << std::endl;
	}

	//std::string font_path = "src/res/Font/Antonio-Bold.ttf";
	std::string font_path = "src/res/Font/Antonio-Regular.ttf";

	if (font_path.empty())
	{
		std::cout << "Error::FreeType : failed to load font_path" << std::endl;
	}


	FT_Face face;
	if (FT_New_Face(ft, font_path.c_str(), 0, &face))
	{
		std::cout << "Error::FreeType: Failed to load font" << std::endl;
	}
	else
	{
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//for (GLubyte i = 0; i < 128; i++)
	//{
	//	if (FT_Load_Char(face, i, FT_LOAD_RENDER))
	//	{
	//		std::cout << "Error::FreeType: Failed to load Glyph" << std::endl;
	//		continue;
	//	}
	//	unsigned int tmp_texture;
	//	GLCall(glGenTextures(1, &tmp_texture));
	//	GLCall(glBindTexture(GL_TEXTURE_2D, tmp_texture));
	//	GLCall(glTexImage2D(
	//		GL_TEXTURE_2D,
	//		0,
	//		GL_RED,
	//		face->glyph->bitmap.width,
	//		face->glyph->bitmap.rows,
	//		0,
	//		GL_RED,
	//		GL_UNSIGNED_BYTE,
	//		face->glyph->bitmap.buffer
	//	));
	//
	//	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	//	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	//	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//
	//	m_character = {
	//		tmp_texture,
	//		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	//		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	//		static_cast<unsigned int>(face->glyph->advance.x)
	//	};
	//	Characters.insert(std::pair<char, Character>(i, m_character));
	//}
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	//VAO, VBO
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));

	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glBindVertexArray(0);
}

void Text::RenderText( std::string text, float x, float y, float scale, glm::vec3 color)
{
	m_Shader.Bind();
	m_Shader.SetUniformVec3("textColor", color.x, color.y, color.z);
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindVertexArray(VAO));

	//std::string::const_iterator c;
	//for (c = text.begin(); c!= text.end(); c++)
	//{
	//	Character ch = Characters[*c];
	//
	//	float xpos = x + ch.Size.x * scale;
	//	float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
	//
	//	float w = ch.Size.x * scale;
	//	float h = ch.Size.y * scale;
	//
	//	float vertices[6][4] = {
	//		{xpos,     ypos + h, 0.0f, 0.0f},
	//		{xpos,     ypos	   , 0.0f, 1.0f},
	//		{xpos + w, ypos    , 1.0f, 1.0f},
	//
	//		{xpos,	   ypos + h, 0.0f, 0.0f},
	//		{xpos + w, ypos    , 1.0f, 1.0f},
	//		{xpos + w, ypos + h, 1.0f, 0.0f},
	//
	//	};
	//
	//	GLCall(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
	//
	//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	//	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
	//
	//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//
	//	x += (ch.Advance >> 6) * scale;
	//}
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get a
	}
	GLCall(glBindVertexArray(0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	

}

Text::~Text()
{
	
}
