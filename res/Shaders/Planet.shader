#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoords;


out vec3 FragPos;
out vec3 Normal;
out vec2 v_texcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;



void main()
{
	FragPos = vec3(model * vec4(a_position, 1.0));
	v_texcoords = a_texcoords;
	Normal = mat3(transpose(inverse(model))) * a_normal;

	gl_Position = projection * view * model * vec4(a_position, 1.0f);
};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

out vec4 FragColor;

in vec2 v_texcoords;
in vec3 FragPos;
in vec3 Normal;
uniform float shininess;

struct DirLight //平行光
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight dirLight;

vec3 CalDirLight(DirLight Light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalDirLight(dirLight, norm, viewDir);
	FragColor = vec4(result, 1.0);
	//FragColor = texture(texture_diffuse1, v_texcoords);
};

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	//漫射计算
	float diff = max(dot(normal, lightDir), 0.0);

	//高光计算
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, v_texcoords));
	vec3 diffuse = light.diffuse * vec3(texture(texture_diffuse1, v_texcoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_diffuse1, v_texcoords));
	return (ambient + diffuse + specular);
	
}