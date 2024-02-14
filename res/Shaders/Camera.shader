#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 TexCoord;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model  * vec4(a_position , 1.0f);
	TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
};

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

//in vec2 TexCoords;
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.1);
};