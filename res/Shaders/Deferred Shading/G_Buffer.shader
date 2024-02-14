#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in mat4 aInstanceMatrix;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //vec4 worldPos = model * vec4(aPos, 1.0);
    vec4 worldPos = aInstanceMatrix * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;

    //mat3 normalMatrix = transpose(inverse(mat3(model)));
    mat3 normalMatrix = transpose(inverse(mat3(aInstanceMatrix)));
    Normal = normalMatrix * aNormal;

    gl_Position = projection * view * worldPos;
}

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // �洢position����Ϣ����һ����ɫ���� ��ʽΪrgb
    gPosition = FragPos;
    // ���淨��������Ϣ���ڶ�����ɫ���� ��ʽΪrgb
    gNormal = normalize(Normal);
    // ������������ͼ����Ҫ����ɫ������������ɫ���� ��ʽΪrgb
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // ����߹���Ϣ����������ɫ���� ��ʽΪa
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}