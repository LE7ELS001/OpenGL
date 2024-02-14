#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;

out vec3 v_TexCoord;

uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	v_TexCoord = a_position; //因为物体中心和原点重合 所以位置的坐标能作为纹理坐标
	vec4 pos = projection * view * vec4(a_position, 1.0);
	gl_Position = pos.xyww; //这里最后的ww好像是为了让他的深度测试永远是1.0 永远在别的物体后面

}

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

out vec4 FragColor;

in vec3 v_TexCoord;

uniform samplerCube skybox;



void main()
{
	FragColor = texture(skybox, v_TexCoord);
}