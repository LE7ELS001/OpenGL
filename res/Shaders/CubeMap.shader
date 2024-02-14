#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;

out vec3 v_TexCoord;

uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	v_TexCoord = a_position; //��Ϊ�������ĺ�ԭ���غ� ����λ�õ���������Ϊ��������
	vec4 pos = projection * view * vec4(a_position, 1.0);
	gl_Position = pos.xyww; //��������ww������Ϊ����������Ȳ�����Զ��1.0 ��Զ�ڱ���������

}

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;

in vec3 v_TexCoord;

uniform samplerCube skybox;



void main()
{
	FragColor = texture(skybox, v_TexCoord);
}