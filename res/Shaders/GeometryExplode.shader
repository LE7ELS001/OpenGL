#Shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_TexCoords;

out VS_OUT{
	vec2 v_texCoords;
	vec3 v_position;
	
}vs_out;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vs_out.v_texCoords = a_TexCoords;
	vs_out.v_position = a_position;
	gl_Position = projection * view * model * vec4(a_position, 1.0);
};






#Shader geometry //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 v_texCoords;
	vec4 v_position;
} gs_in[];

out vec2 f_TexCoords;

uniform float time;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0f;
	vec3 direction = normal * ((sin(time) + 1.0) * 0.5)* magnitude;
	return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
	vec3 pos0 = vec3(gs_in[0].v_position);
	vec3 pos1 = vec3(gs_in[1].v_position);
	vec3 pos2 = vec3(gs_in[2].v_position);

	vec3 a = pos0 - pos1;
	vec3 b = pos2 - pos1;
	return normalize(cross(b, a));
}

void main() {
	mat4 mvp = projection * view * model;
	vec3 normal = GetNormal();
	vec4 pos = gs_in[0].v_position;
	gl_Position = mvp * explode(pos, normal);
	f_TexCoords = gs_in[0].v_texCoords;
	EmitVertex();

	pos = gs_in[1].v_position;
	gl_Position = mvp * explode(pos, normal);
	f_TexCoords = gl_in[1].v_texCoords;
	EmitVertex();

	pos = gs_in[2].v_position;
	gl_Position = mvp * explode(pos, normal);
	f_TexCoords = gl_in[2].v_texCoords;
	EmitVertex();
	EndPrimitive();
}



//Ƭ����ɫ��Դ��
#Shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ


out vec4 FragColor;

in vec2 f_TexCoords;

uniform sampler2D texture_diffuse1;



void main()
{
	FragColor = texture(texture_diffuse1, f_TexCoords);
};
