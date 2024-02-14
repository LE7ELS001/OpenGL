#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_offset;


out vec3 v_color;


void main()
{
	v_color = a_color;
	gl_Position = vec4(a_position + a_offset, 0.0, 1.0);
};

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;
in vec3 v_color;

void main()
{
	FragColor = vec4(v_color, 1.0);
};