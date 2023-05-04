#shader vertex

#version 330 core

layout(location = 0) in vec3 position;//位置变量的属性位置值为0
layout(location = 1) in vec3 aColor;//位置变量的属性位置值为1

out vec3 ourColor;//向片段着色器输出一个颜色

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