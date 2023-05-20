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
