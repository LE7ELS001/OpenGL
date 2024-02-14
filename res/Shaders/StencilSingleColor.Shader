#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_TexCoords;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(a_position, 1.0f);

};

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

//in vec2 TexCoords;
out vec4 FragColor;


void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);
	FragColor = vec4(0.04,0.28,0.26,1.0);
};