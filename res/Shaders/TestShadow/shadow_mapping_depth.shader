#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

void main()
{
    // gl_FragDepth = gl_FragCoord.z;
}
