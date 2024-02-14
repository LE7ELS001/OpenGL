#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_TexCoord;



out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	Normal = mat3(transpose(inverse(model))) * a_TexCoord; //获取法向量 假设为N
	Position = vec3(model * vec4(a_position, 1.0)); //世界空间坐标 用于在片段着色器中计算观察方向的向量 这里假设为I 
	gl_Position = projection * view * model * vec4(a_position, 1.0);

}

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
uniform vec3 cameraPos;
uniform samplerCube skybox;



void main()
{
	//反射
	//vec3 I = normalize(Position - cameraPos);
	//vec3 R = reflect(I, normalize(Normal));
	//FragColor = vec4(texture(skybox, R).rgb, 1.0);

	//折射
	float ratio = 1.00 / 1.52; // 后面那个1.52是有表格查询的 不同的物质折射率是不一样的 比如水和玻璃的折射率就不同
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal),ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
