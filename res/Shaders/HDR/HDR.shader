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

//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//uniform sampler2D hdrBuffer;
//uniform bool hdr;
//uniform float exposure;
//
//void main()
//{
//    const float gamma = 2.2;
//    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
//    if (hdr)
//    {
//        // 尽可能将HDR的所有信息转成[0,1]的方程
//        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
//        
//        // 加入曝光
//        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
//              
//        result = pow(result, vec3(1.0 / gamma));
//        FragColor = vec4(result, 1.0);
//    }
//    else
//    {
//        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
//        FragColor = vec4(result, 1.0);
//    }
//}

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if (hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}