#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if (bloom)
        hdrColor += bloomColor; //把模糊后的效果和本来的光效合一起
    // 结合曝光的色调映射
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}