#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    WorldPos = aPos;
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}


//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

//out vec4 FragColor;
//in vec3 WorldPos;
//
//uniform sampler2D equirectangularMap;
//
//const vec2 invAtan = vec2(0.1591, 0.3183);
//vec2 SampleSphericalMap(vec3 v)
//{
//    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
//    uv *= invAtan;
//    uv += 0.5;
//    return uv;
//}
//
//void main()
//{
//    vec2 uv = SampleSphericalMap(normalize(WorldPos));
//    vec3 color = texture(equirectangularMap, uv).rgb;
//
//    FragColor = vec4(color, 1.0);
//}
out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(WorldPos));
    vec3 color = texture(equirectangularMap, uv).rgb;

    FragColor = vec4(color, 1.0);
}

