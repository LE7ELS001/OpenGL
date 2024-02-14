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

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// 半球的各项参数 改变可以出现不同效果
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// 裁剪空间转回到屏幕空间的方程（这部分没太懂）
const vec2 noiseScale = vec2(960.0 / 4.0, 540.0 / 4.0);

//这个就是从观察空间到裁剪空间
uniform mat4 projection;

void main()
{
    // 从G_buffer里面 获取各种数据 位置 法线 
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    //好像是一个叫Orthogonal Basis正交基的东西 没太懂 貌似和TBN有关
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // TBN 转换切线空间到观察空间 这里用到了上面的randomvec 原文说的是不需要一个沿着集合体表面的TBN
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // 计算环境光遮蔽
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i)
    {
        // 获取采样位置
        vec3 samplePos = TBN * samples[i]; 
        samplePos = fragPos + samplePos * radius;

        //好像是把采样的样本投射到物体上 
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // 从观察空间到裁剪空间
        offset.xyz /= offset.w; // 透视划分 让平面出现透视效果 
        offset.xyz = offset.xyz * 0.5 + 0.5; // 因为深度对比是[0,1] 所以要转变一下

        // 获取采样深度
        float sampleDepth = texture(gPosition, offset.xy).z; 
        // rangeCheck是指限定一个范围 不然人物里墙有距离依旧出现黑边会让空间感很怪  
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        //在范围内对比深度 决定哪一个在前后 
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    //这个输出的只是环境光遮蔽的一个结果 后面计算光照才会用到
    FragColor = occlusion;
}