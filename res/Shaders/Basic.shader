#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec4 v_color;
out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 m_MVP; //model view projection 

void main()
{
	gl_Position = m_MVP * a_position ;
	v_color = a_color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

in vec2 TexCoords;
layout(location = 0)out vec4 color;


//in vec4 v_color;
in vec2 v_TexCoord;
in float v_TexIndex;


//uniform vec4 u_color;
uniform sampler2D u_Texture[2];

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);
	int index = int(v_TexIndex);
	color = texture(u_Texture[index], v_TexCoord);
	
	


};