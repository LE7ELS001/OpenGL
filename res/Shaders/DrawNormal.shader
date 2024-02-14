#Shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out VS_OUT{
	vec3 normal;
} vs_out;

uniform mat4 view;
uniform mat4 model;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));
	gl_Position = view * model * vec4(aPos, 1.0);
}



#Shader geometry //几何着色器
#version 330 core //版本330 并且是核心模式


//layout(triangles) in;
//layout(line_strip, max_vertices = 6) out;

//in VS_OUT{
//	vec3 v_Normal;
//} gs_in[];
//
//const float MAGNITUDE = 0.2;
//
//uniform mat4 projection; 
//
//void GenerateLine(int index)
//{
//	gl_Position = projection * gl_in[index].gl_Position;
//	EmitVertex();
//	gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].v_Normal, 0, 0) * MAGNITUDE);
//	EmitVertex();
//	EndPrimitive();
//}
//
//void main() {
//	GenerateLine(0);
//	GenerateLine(1);
//	GenerateLine(2);
//}

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT{
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}




//片段着色器源码
#Shader fragment

#version 330 core //版本330 并且是核心模式


out vec4 FragColor;


void main()
{
	FragColor = vec4(1.0,1.0,0.0,1.0);
};