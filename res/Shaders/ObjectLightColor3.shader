#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_textureCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 v_textureCoord;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	FragPos = vec3(model * vec4(a_position, 1.0)); //�����������е� Ƭ��λ��
	Normal = mat3(transpose(inverse(model))) * a_normal; //�����������е� ���ߵ�λ��
	v_textureCoord = a_textureCoord;

	gl_Position = projection * view * model * vec4(FragPos, 1.0);

};

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

//in vec2 TexCoords;
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 v_textureCoord;

struct Material   //�൱��֮ǰ��object //������vec3 ��Ϊ�������������ʽ ������sampler2d //����û��ambient ��Ϊ���������� ambient��diffuse��ֵ���
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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
uniform float matrixmove;
uniform float matrixlight;

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);

	//������
	vec3 ambient = light.ambient * texture(material.diffuse, v_textureCoord).rgb;

	//�����
	vec3 norm = normalize(Normal); //��׼������
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_textureCoord).rgb;

	//�߹�
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-light.position, FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shineness); //32�Ƿ���ֵ ֵԽ�� ����Խ����  ͨ��������shinenes *128  ��ֵ��һ�������Բ�
	vec3 specular = light.specular * spec * texture(material.specular, v_textureCoord).rgb;
	vec3 emission = matrixlight *texture(material.emission, vec2(v_textureCoord.x, v_textureCoord.y + matrixmove)).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
};