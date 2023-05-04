#shader vertex

#version 330 core

layout(location = 0) in vec3 position;//λ�ñ���������λ��ֵΪ0
layout(location = 1) in vec3 aColor;//λ�ñ���������λ��ֵΪ1

out vec3 ourColor;//��Ƭ����ɫ�����һ����ɫ

void main()
{
	gl_Position = vec4(position , 1.0);
	ourColor = aColor;
}

#shader fragment

#version 330 core

out vec4 color;
in vec3 ourColor;

uniform vec3 dy;

void main()
{
	color = vec4(dy * ourColor, 1.0);
}