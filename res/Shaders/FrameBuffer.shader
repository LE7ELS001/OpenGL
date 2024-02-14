#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_TexCoord;



out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_position, 0.0, 1.0); //�����ûر�׼���Ǹ�λ������
} 

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D ScreenTexture;


const float offset = 1.0 / 300.0;

void main()
{
	
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, -offset)
		);
	
	float kernel[9] = float[](
		1.0/16, 2.0/16, 1.0/16,
		2.0/16, 4.0/16, 2.0/16,
		1.0/16, 2.0/16, 1.0/16
		);
	
	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(ScreenTexture, v_TexCoord.st + offsets[i]));
	}
	
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		col += sampleTex[i] * kernel[i];
	}
	
	FragColor = vec4(col, 1.0);



	
}
