#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_TexCoord;



out vec2 v_TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = projection * view * model * vec4(a_position, 1.0);

}

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D texturel;

void main()
{
	FragColor = texture(texturel, v_TexCoord);
}

//vec4 TexColor = texture(texturel, v_TexCoord); //��һ����˵ �����������alphaֵ��͸��ֵ�� С�� 0.1�Ļ�ֱ�Ӷ��� �ݵ��������� С��0.1���ǰ�ɫ�Ĳ��� ��ô���ֻ��ʣ����ɫ�ݵĲ���
	//if (TexColor.a < 0.1f)
		//discard;
