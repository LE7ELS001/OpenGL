#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec2 a_position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
	v_TexCoord = a_TexCoord;
};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式


out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D screenTexture;


void main()
{
	vec3 col = texture(screenTexture, v_TexCoord).rgb;
	float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;

	FragColor = vec4(vec3(grayscale),1.0);




};