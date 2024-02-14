#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;

    gl_Position = projection * view * vec4(WorldPos, 1.0);
}


//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// 各项材质属性
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// 光源
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
//BRDF公式中的D 法线分布函数 与高光有关 
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
//BRDF公式中的G 几何函数 物体表面产生自遮挡的现象 主要有两个点 一个是和灯光的方向有关 一个是和你视线的位置有关
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
//史密斯的GGX计算 调用到了上面的施里克GGX 
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
//菲涅尔方程 其中金属度为0时物体是黑色 金属度为1时 物体是自身漫反射的颜色
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

       
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 4; ++i)
    {
        // 计算灯的辐射率
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        //BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001避免分母为0
        vec3 specular = numerator / denominator;

        //高光的比例 和菲涅尔有关 可以直接用F
        vec3 kS = F;
        // 漫反射的比例 根据能量守恒 折射和反射的总和不能超过1 那么总量为1 已知高光一个可以算另一个
        vec3 kD = vec3(1.0) - kS;
        // 漫反射还和金属度有关 因为纯金属是不会受漫反射影响
        kD *= 1.0 - metallic;

        // 好像是灯光的强度 不然会很暗
        float NdotL = max(dot(N, L), 0.0);

        // 整合到一起
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  
        //好像是因为 LO是累加所有光线的作用 菲涅尔反射是描述光线在表面上的反射与折射之间的关系 所以再specular中已经计算好了 所以没有再乘上KS
    }

    
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR 
    color = color / (color + vec3(1.0));
    // 伽马校正
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
