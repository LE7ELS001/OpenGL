#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式
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

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

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
    // 存储position的信息到第一个颜色附件 格式为rgb
    gPosition = FragPos;
    // 储存法向量的信息到第二个颜色附件 格式为rgb
    gNormal = normalize(Normal);
    // 储存漫反射贴图（主要是颜色）到第三个颜色附件 格式为rgb
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // 储存高光信息到第三个颜色附件 格式为a
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}