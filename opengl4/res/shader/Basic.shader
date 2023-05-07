#shader vertex

#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 acolor;
out vec2 v_TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * position;
    acolor = color;
    v_TexCoord = texCoord;
}

#shader fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec3 acolor;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform int isColor = 0;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixval;

void main()
{
    vec4 texColor;
    if (isColor != 0)
    {
        texColor = mix(texture(texture0, v_TexCoord), texture(texture1, v_TexCoord), mixval) * u_Color;
    }
    else
        texColor = mix(texture(texture0, v_TexCoord), texture(texture1, v_TexCoord), mixval);
    color = texColor;
}