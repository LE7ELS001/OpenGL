#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;  //model view projection 
uniform mat4 view;
uniform mat4 projection;


void main()
{
	FragPos = vec3(model * vec4(a_position, 1.0)); //在世界坐标中的 片段位置
	Normal = mat3(transpose(inverse(model))) * a_normal; //在世界坐标中的 法线的位置

	gl_Position = projection * view * model * vec4(FragPos, 1.0);

};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

//in vec2 TexCoords;
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material   //相当于之前的object
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
uniform vec3 viewPos; //视角位置（摄像机位置） 世界坐标

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);

	//环境光
	vec3 ambient = light.ambient * material.ambient;

	//漫射光
	vec3 norm = normalize(Normal); //标准化向量
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-light.position ,FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shineness); //32是发光值 值越高 反射越明显  通常计算是shinenes *128  数值有一个表格可以查
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
};