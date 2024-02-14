#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

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


//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// �����������
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// ��Դ
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
//BRDF��ʽ�е�D ���߷ֲ����� ��߹��й� 
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
//BRDF��ʽ�е�G ���κ��� �������������ڵ������� ��Ҫ�������� һ���Ǻ͵ƹ�ķ����й� һ���Ǻ������ߵ�λ���й�
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
//ʷ��˹��GGX���� ���õ��������ʩ���GGX 
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
//���������� ���н�����Ϊ0ʱ�����Ǻ�ɫ ������Ϊ1ʱ �������������������ɫ
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
        // ����Ƶķ�����
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
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001�����ĸΪ0
        vec3 specular = numerator / denominator;

        //�߹�ı��� �ͷ������й� ����ֱ����F
        vec3 kS = F;
        // ������ı��� ���������غ� ����ͷ�����ܺͲ��ܳ���1 ��ô����Ϊ1 ��֪�߹�һ����������һ��
        vec3 kD = vec3(1.0) - kS;
        // �����仹�ͽ������й� ��Ϊ�������ǲ�����������Ӱ��
        kD *= 1.0 - metallic;

        // �����ǵƹ��ǿ�� ��Ȼ��ܰ�
        float NdotL = max(dot(N, L), 0.0);

        // ���ϵ�һ��
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  
        //��������Ϊ LO���ۼ����й��ߵ����� ���������������������ڱ����ϵķ���������֮��Ĺ�ϵ ������specular���Ѿ�������� ����û���ٳ���KS
    }

    
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR 
    color = color / (color + vec3(1.0));
    // ٤��У��
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
