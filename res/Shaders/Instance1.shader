#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_offset;


out vec3 v_color;


void main()
{
	v_color = a_color;
	gl_Position = vec4(a_position + a_offset, 0.0, 1.0);
};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

out vec4 FragColor;
in vec3 v_color;

void main()
{
	FragColor = vec4(v_color, 1.0);
};