#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;

uniform mat4 projection;

void main()
{
	TexCoords = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{

	color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}