#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct SpotLight {

    vec3  position;
    vec3  direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//��ֱ��ɢ�Ĳ���
    float linea;
    float quadratic;

    float cutOff;//����Բ׶�Ƕ�ȡcos
    float outerCutOff;
};

uniform SpotLight spotlight;
uniform vec3 viewPos;

void main()
{    
    
    FragColor = texture(texture_diffuse1, TexCoords);
}