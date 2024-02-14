#Shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aPos, 1.0);
}



#Shader geometry //几何着色器
#version 330 core //版本330 并且是核心模式


layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; 

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face; //gl_Layer 指定发散出基本图形送到立方体贴图的哪一个面
        for (int i = 0; i < 3; ++i) 
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}


//片段着色器源码
#Shader fragment

#version 330 core //版本330 并且是核心模式

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);

    // 保证这个数为[0,1]
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}