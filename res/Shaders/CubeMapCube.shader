#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_TexCoord;



out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	Normal = mat3(transpose(inverse(model))) * a_TexCoord; //��ȡ������ ����ΪN
	Position = vec3(model * vec4(a_position, 1.0)); //����ռ����� ������Ƭ����ɫ���м���۲췽������� �������ΪI 
	gl_Position = projection * view * model * vec4(a_position, 1.0);

}

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
uniform vec3 cameraPos;
uniform samplerCube skybox;



void main()
{
	//����
	//vec3 I = normalize(Position - cameraPos);
	//vec3 R = reflect(I, normalize(Normal));
	//FragColor = vec4(texture(skybox, R).rgb, 1.0);

	//����
	float ratio = 1.00 / 1.52; // �����Ǹ�1.52���б���ѯ�� ��ͬ�������������ǲ�һ���� ����ˮ�Ͳ����������ʾͲ�ͬ
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal),ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
