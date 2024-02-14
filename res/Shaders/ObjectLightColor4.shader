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
	float shineness;
};

struct Light
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//衰减公式 Attenuation = Constant + Linear * Distance + Quadratic * Distance^2
	float constant; 
	float Linear;  
	float quadratic;

	float cutOff; //内圈
	float outerCutOff; //外圈 
};


uniform Material material;
uniform Light light;
uniform vec3 viewPos; //视角位置（摄像机位置） 世界坐标


void main()
{
	//vec4 texColor = texture(u_Texture, TexCoords);

	//环境光
	vec3 ambient = light.ambient * texture(material.diffuse, v_textureCoord).rgb;

	//漫射光
	vec3 norm = normalize(Normal); //标准化向量
	vec3 lightDir = normalize(light.position - FragPos );
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_textureCoord).rgb;

	//高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); //之前写成-Light.direction了
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shineness); //32是发光值 值越高 反射越明显  通常计算是shinenes *128  数值有一个表格可以查
	vec3 specular = light.specular * spec * texture(material.specular, v_textureCoord).rgb;


	//超过内圈 没到外圈之间的过度公式
	float theta = dot(lightDir, normalize(-light.direction)); //没记错好像是返回一个cosX的值
	float epsilon =  (light.cutOff - light.outerCutOff); // 因为返回的是cosx的值 cos越接近0反而越大 所以越大的radian然而越小
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //没记错outerCutOff好像也是cosY之类的值 并不是角度 公式 i = theta - outercutoff / epsilon
	




	diffuse *= intensity;
	specular *= intensity;


	//公式中的d
	float distance = length(light.position - FragPos);
	float Attenuation = 1.0 / (light.constant + distance * light.Linear + light.quadratic * (distance * distance));

	ambient *= Attenuation;
	diffuse *= Attenuation;
	specular *= Attenuation;

	vec3 result = ambient + diffuse + specular ;
	FragColor = vec4(result, 1.0);
};