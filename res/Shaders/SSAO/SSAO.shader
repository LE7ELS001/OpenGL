#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}


//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// ����ĸ������ �ı���Գ��ֲ�ͬЧ��
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// �ü��ռ�ת�ص���Ļ�ռ�ķ��̣��ⲿ��û̫����
const vec2 noiseScale = vec2(960.0 / 4.0, 540.0 / 4.0);

//������Ǵӹ۲�ռ䵽�ü��ռ�
uniform mat4 projection;

void main()
{
    // ��G_buffer���� ��ȡ�������� λ�� ���� 
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    //������һ����Orthogonal Basis�������Ķ��� û̫�� ò�ƺ�TBN�й�
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // TBN ת�����߿ռ䵽�۲�ռ� �����õ��������randomvec ԭ��˵���ǲ���Ҫһ�����ż���������TBN
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // ���㻷�����ڱ�
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i)
    {
        // ��ȡ����λ��
        vec3 samplePos = TBN * samples[i]; 
        samplePos = fragPos + samplePos * radius;

        //�����ǰѲ���������Ͷ�䵽������ 
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // �ӹ۲�ռ䵽�ü��ռ�
        offset.xyz /= offset.w; // ͸�ӻ��� ��ƽ�����͸��Ч�� 
        offset.xyz = offset.xyz * 0.5 + 0.5; // ��Ϊ��ȶԱ���[0,1] ����Ҫת��һ��

        // ��ȡ�������
        float sampleDepth = texture(gPosition, offset.xy).z; 
        // rangeCheck��ָ�޶�һ����Χ ��Ȼ������ǽ�о������ɳ��ֺڱ߻��ÿռ�кܹ�  
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        //�ڷ�Χ�ڶԱ���� ������һ����ǰ�� 
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    //��������ֻ�ǻ������ڱε�һ����� ���������ղŻ��õ�
    FragColor = occlusion;
}