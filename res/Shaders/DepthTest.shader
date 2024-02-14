#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;
//layout(location = 1) in vec2 a_TexCoord;



//out vec2 v_TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	//v_TexCoord = a_TexCoord;
	gl_Position = projection * view * model * vec4(a_position, 1.0);

}

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

out vec4 FragColor;

float near = 0.1f;
float far = 100.0f; 



float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;			//让坐标从[0,1] 变成[-1,1]
	return (2.0 * near * far) / (far + near - z * (far - near));

}



void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	FragColor = vec4(vec3(depth), 1.0);
}