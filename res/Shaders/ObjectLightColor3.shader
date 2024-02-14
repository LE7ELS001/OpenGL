#shader vertex //不要空行否则会抛异常 似乎和NONE = -1 有关 //顶点着色器
#version 330 core //版本330 并且是核心模式

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
	FragPos = vec3(model * vec4(a_position, 1.0)); //在世界坐标中的 片段位置
	Normal = mat3(transpose(inverse(model))) * a_normal; //在世界坐标中的 法线的位置
	v_textureCoord = a_textureCoord;

	gl_Position = projection * view * model * vec4(FragPos, 1.0);

};

//片段着色器源码
#shader fragment

#version 330 core //版本330 并且是核心模式

//in vec2 TexCoords;
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 v_textureCoord;

struct Material   //相当于之前的object //不能用vec3 因为是类似纹理的形式 所以是sampler2d //而且没有ambient 因为大多数情况下 ambient和diffuse的值相近
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
uniform vec3 viewPos; //视角位置（摄像机位置） 世界坐标
uniform float matrixmove;
uniform float matrixlight;

void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);

	//环境光
	vec3 ambient = light.ambient * texture(material.diffuse, v_textureCoord).rgb;

	//漫射光
	vec3 norm = normalize(Normal); //标准化向量
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_textureCoord).rgb;

	//高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-light.position, FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shineness); //32是发光值 值越高 反射越明显  通常计算是shinenes *128  数值有一个表格可以查
	vec3 specular = light.specular * spec * texture(material.specular, v_textureCoord).rgb;
	vec3 emission = matrixlight *texture(material.emission, vec2(v_textureCoord.x, v_textureCoord.y + matrixmove)).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
};