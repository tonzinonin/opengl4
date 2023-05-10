#shader vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 TexPos;

out vec3 Normal;
out vec3 FragPos;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position , 1.0);
    FragPos = vec3(model * vec4(position , 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    texCoords = TexPos;
}


#shader fragment

#version 330 core

layout(location = 0) out vec4 color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D glow;
    float shininess;
};

struct flashLight {

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

uniform flashLight light;
uniform Material material;
uniform vec3 viewPos;
//uniform float movement;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoords;

void main()
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linea * distance
        + light.quadratic * (distance * distance));

    vec3 norm = normalize(Normal);//�������ĵ�λ����
    vec3 lightDir = normalize(light.position - FragPos);//�ֲ��ռ�������ָ��ƹ�ĵ�λ����

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);//����������ǿ��

    vec3 viewDir = normalize(viewPos - FragPos);//����ָ��������ĵ�λ����
    vec3 reflectDir = reflect(-lightDir, norm);//�ƹ�ľ��淴������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //vec2 glowPosition = vec2( texCoords.x , (texCoords.y + movement * 0.2));

    vec3 ambient = vec3(texture(material.diffuse, texCoords)) * light.ambient;//������--
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));
    //vec3 glowing = vec3(texture(material.glow, glowPosition));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;//��ֱ˥��

    diffuse *= intensity;
    specular *= intensity;//ˮƽ˥��

    //float edge = 0.08;
    vec3 result = ambient + diffuse + specular;
    //    ((texCoords.x < edge || texCoords.y > (1 - edge) || texCoords.x > (1 - edge) || texCoords.y < edge) ?
    //        vec3(0 , 0 , 0) : glowing);
    color = vec4(result, 1.0);
}