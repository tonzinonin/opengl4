#shader vertex

#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 scale;

void main()
{
    gl_Position = projection * view * model * scale * vec4(position, 1.0);
}


#shader fragment

#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0);
}