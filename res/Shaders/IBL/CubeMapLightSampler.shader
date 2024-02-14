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
//uniform samplerCube environmentMap;
//
//const float PI = 3.14159265359;
//
//void main()
//{
//    //好像是利用顶点坐标来做法线
//    vec3 N = normalize(WorldPos);
//
//    vec3 irradiance = vec3(0.0);
//
//    // 计算TBN 后面采样要用到
//    vec3 up = vec3(0.0, 1.0, 0.0);
//    vec3 right = normalize(cross(up, N));
//    up = normalize(cross(N, right));
//
//    float sampleDelta = 0.025;
//    float nrSamples = 0.0;
//    //设定采样的大小 
//    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
//    {
//        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
//        {
//            
//            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
//            // 这里就是用上刚刚的TBN 转到世界坐标去采样
//            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
//
//            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
//            nrSamples++;
//        }
//    }
//    irradiance = PI * irradiance * (1.0 / float(nrSamples));
//
//    FragColor = vec4(irradiance, 1.0);
//}
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
    vec3 N = normalize(WorldPos);

    vec3 irradiance = vec3(0.0);

    // tangent space calculation from origin point
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0f;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    FragColor = vec4(irradiance, 1.0);
}
