#shader vertex

#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * position;
    v_TexCoord = texCoord;
}


#shader fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixval;

void main()
{
    vec4 texColor;
    texColor = mix(texture(texture0, v_TexCoord) , texture(texture1, v_TexCoord) , mixval);
    color = texColor;
}