#Shader vertex //��Ҫ���з�������쳣 �ƺ���NONE = -1 �й� //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec3 a_color;

out VS_OUT{
	vec3 v_color;
}vs_out;




void main()
{
	vs_out.v_color = a_color;
	gl_Position = vec4(a_position.x, a_position.y, 0.0,1.0);
};



#Shader geometry //������ɫ��
#version 330 core //�汾330 �����Ǻ���ģʽ


layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec3 v_color;
} gs_in[];

out vec3 fColor;

void draw_house(vec4 position)
{
	fColor = gs_in[0].v_color; 
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);   
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0); 
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); 
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0); 
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0); 
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main() {
	draw_house(gl_in[0].gl_Position);
}




//Ƭ����ɫ��Դ��
#Shader fragment

#version 330 core //�汾330 �����Ǻ���ģʽ


out vec4 FragColor;

in vec3 fColor;




void main()
{
	FragColor = vec4(fColor, 1.0);
};