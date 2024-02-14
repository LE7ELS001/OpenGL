#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(a_position, 1.0f);

};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

//in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);
	FragColor = vec4(lightColor * objectColor, 1.0);
};