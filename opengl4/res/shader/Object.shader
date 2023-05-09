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

    vec3 ambient = material.ambient * light.ambient;//环境光--

    vec3 norm = normalize(Normal);//法向量的单位向量
    vec3 lightDir = normalize(light.position - FragPos);//局部空间中物体指向灯光的单位向量
    float diff = max(dot(norm, lightDir), 0.0);//计算漫反射强度
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;//漫反射--添加了光下表面颜色

    //镜面光
    vec3 viewDir = normalize(viewPos - FragPos);//物体指向摄像机的单位向量
    vec3 reflectDir = reflect(-lightDir, norm);//灯光的镜面反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}