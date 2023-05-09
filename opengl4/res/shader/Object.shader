#shader vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position , 1.0);
    FragPos = vec3(model * vec4(position , 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}


#shader fragment

#version 330 core

layout(location = 0) out vec4 color;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;


in vec3 Normal;
in vec3 FragPos;


void main()
{

    vec3 ambient = material.ambient * light.ambient;//������--

    vec3 norm = normalize(Normal);//�������ĵ�λ����
    vec3 lightDir = normalize(light.position - FragPos);//�ֲ��ռ�������ָ��ƹ�ĵ�λ����
    float diff = max(dot(norm, lightDir), 0.0);//����������ǿ��
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;//������--����˹��±�����ɫ

    //�����
    vec3 viewDir = normalize(viewPos - FragPos);//����ָ��������ĵ�λ����
    vec3 reflectDir = reflect(-lightDir, norm);//�ƹ�ľ��淴������
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}