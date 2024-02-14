#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 TexCoord;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model  * vec4(a_position , 1.0f);
	TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

//in vec2 TexCoords;
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.1);
};