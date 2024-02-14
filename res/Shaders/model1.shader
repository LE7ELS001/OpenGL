#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_textureCoord;

out vec3 FragPos;
out vec2 v_textureCoord;
out vec3 Normal;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	FragPos = vec3(model * vec4(a_position, 1.0));
	Normal = mat3(transpose(inverse(model))) * a_normal;
	v_textureCoord = a_textureCoord;
	gl_Position = projection * view * model * vec4(a_position, 1.0);

};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

//in vec2 TexCoords;
out vec4 FragColor;


in vec2 v_textureCoord;
in vec3 FragPos;
in vec3 Normal;


struct PointLight //点光
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};


//uniform Material material;
//uniform Light light;
uniform vec3 viewPos; //视角位置（摄像机位置） 世界坐标

#define NR_POINT_LIGHTS 4

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform float shininess;
uniform PointLight pointLight[NR_POINT_LIGHTS];


vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalPointLight2(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//这边这样写两个函数其实并不太好 我查了下最好是根据纹理单元去决定到底传入texture_diffuse1还是texture——diffuse2 但是纹理单元和刚刚那步有关 我没太懂

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	vec3 result2;
	vec3 finalResult;

	for ( int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		if (textureSize(texture_diffuse2, 0).x > 0)
			//检测是否有传入diffuse2
		{
			//有传入
			result += CalPointLight(pointLight[i], norm, FragPos, viewDir);
			result2 += CalPointLight2(pointLight[i], norm, FragPos, viewDir);
			finalResult = result + result2;
		}
		else
		{
			//没有传入
			result += CalPointLight(pointLight[i], norm, FragPos, viewDir);
			finalResult = result;
		}
	}
	FragColor = vec4(finalResult, 1.0);
};


vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//漫射
	float diff = max(dot(normal, lightDir), 0.0);

	//高光
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));


	//计算
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, v_textureCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, v_textureCoord));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_textureCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalPointLight2(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//漫射
	float diff = max(dot(normal, lightDir), 0.0);

	//高光
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));


	//计算
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse2, v_textureCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse2, v_textureCoord));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_textureCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}