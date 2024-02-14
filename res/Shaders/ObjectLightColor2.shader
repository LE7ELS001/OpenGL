#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	FragPos = vec3(model * vec4(a_position, 1.0)); //�����������е� Ƭ��λ��
	Normal = mat3(transpose(inverse(model))) * a_normal; //�����������е� ���ߵ�λ��

	gl_Position = projection * view * model * vec4(FragPos, 1.0);

};

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

//in vec2 TexCoords;
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material   //�൱��֮ǰ��object
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shineness;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform Material material;
uniform Light light;
uniform vec3 viewPos; //�ӽ�λ�ã������λ�ã� ��������

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);

	//������
	vec3 ambient = light.ambient * material.ambient;

	//�����
	vec3 norm = normalize(Normal); //��׼������
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//�߹�
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-light.position ,FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shineness); //32�Ƿ���ֵ ֵԽ�� ����Խ����  ͨ��������shinenes *128  ��ֵ��һ�������Բ�
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
};