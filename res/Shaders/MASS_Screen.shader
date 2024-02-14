#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec2 a_position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
	v_TexCoord = a_TexCoord;
};

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ


out vec4 FragColor;
in vec2 v_TexCoord;

uniform sampler2D screenTexture;


void main()
{
	vec3 col = texture(screenTexture, v_TexCoord).rgb;
	float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;

	FragColor = vec4(vec3(grayscale),1.0);




};