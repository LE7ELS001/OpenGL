#shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
//layout(location = 1) in vec2 a_TexCoord;



//out vec2 v_TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;//model view projection 

void main()
{
	//v_TexCoord = a_TexCoord;
	gl_Position = projection * view * model * vec4(a_position, 1.0);

}

//Ƭ����ɫ��Դ��
#shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ

out vec4 FragColor;

float near = 0.1f;
float far = 100.0f; 



float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;			//�������[0,1] ���[-1,1]
	return (2.0 * near * far) / (far + near - z * (far - near));

}



void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	FragColor = vec4(vec3(depth), 1.0);
}