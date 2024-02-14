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
//uniform float roughness;
//
//const float PI = 3.14159265359;
//// ----------------------------------------------------------------------------
//float DistributionGGX(vec3 N, vec3 H, float roughness)
//{
//    float a = roughness * roughness;
//    float a2 = a * a;
//    float NdotH = max(dot(N, H), 0.0);
//    float NdotH2 = NdotH * NdotH;
//
//    float nom = a2;
//    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
//    denom = PI * denom * denom;
//
//    return nom / denom;
//}
//// ----------------------------------------------------------------------------
////低差异序列 作用是让生成的采样点更加均匀
//float RadicalInverse_VdC(uint bits)
//{
//    bits = (bits << 16u) | (bits >> 16u);
//    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
//    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
//    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
//    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
//    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
//}
//// ----------------------------------------------------------------------------
////把生成的采样点转成二维平面上的点
//vec2 Hammersley(uint i, uint N)
//{
//    return vec2(float(i) / float(N), RadicalInverse_VdC(i));
//}
//// ----------------------------------------------------------------------------
////把用了低差异序列生成的采样点(2D) 转到球面上(3D)
//vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
//{
//    float a = roughness * roughness;
//
//    float phi = 2.0 * PI * Xi.x;
//    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
//    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
//
//    // 组合生成一个方向
//    vec3 H;
//    H.x = cos(phi) * sinTheta;
//    H.y = sin(phi) * sinTheta;
//    H.z = cosTheta;
//
//    // TBN
//    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
//    vec3 tangent = normalize(cross(up, N));
//    vec3 bitangent = cross(N, tangent);
//
//    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
//    return normalize(sampleVec);
//}
//// ----------------------------------------------------------------------------
//void main()
//{
//    vec3 N = normalize(WorldPos);
//
//    
//    vec3 R = N;
//    vec3 V = R;
//
//    const uint SAMPLE_COUNT = 1024u;
//    vec3 prefilteredColor = vec3(0.0);
//    float totalWeight = 0.0;
//
//    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
//    {
//        //低差异序列生成的采样点
//        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
//        //半球采样点
//        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
//        //并不是直接用H当采样方向 而是法线V基于H上做反射来得出采样向量
//        vec3 L = normalize(2.0 * dot(V, H) * H - V);
//
//        float NdotL = max(dot(N, L), 0.0);
//        if (NdotL > 0.0)
//        {
//            // 输入生成的二维采样点 以及法线 以及粗糙度 得出根据粗糙度生成的采样点
//            float D = DistributionGGX(N, H, roughness);
//            float NdotH = max(dot(N, H), 0.0);
//            float HdotV = max(dot(H, V), 0.0);
//            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;
//
//            float resolution = 512.0; 
//            float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
//            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
//
//            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
//
//            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
//            totalWeight += NdotL;
//        }
//    }
//
//    prefilteredColor = prefilteredColor / totalWeight;
//
//    FragColor = vec4(prefilteredColor, 1.0);
//}
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float roughness;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
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
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}
// ----------------------------------------------------------------------------
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // from spherical coordinates to cartesian coordinates - halfway vector
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space H vector to world-space sample vector
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}
// ----------------------------------------------------------------------------
void main()
{
    vec3 N = normalize(WorldPos);

    // make the simplifying assumption that V equals R equals the normal 
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;

    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)
        {
            // sample from the environment's mip level based on roughness/pdf
            float D = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

            float resolution = 512.0; // resolution of source cubemap (per face)
            float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}