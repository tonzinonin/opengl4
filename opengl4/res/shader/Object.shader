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
    sampler2D glow;
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
uniform float movement;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoords;

void main()
{

    vec3 norm = normalize(Normal);//法向量的单位向量
    vec3 lightDir = normalize(light.position - FragPos);//局部空间中物体指向灯光的单位向量
    float diff = max(dot(norm, lightDir), 0.0);//计算漫反射强度

    vec3 viewDir = normalize(viewPos - FragPos);//物体指向摄像机的单位向量
    vec3 reflectDir = reflect(-lightDir, norm);//灯光的镜面反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec2 glowPosition = vec2( texCoords.x , (texCoords.y + movement * 0.2));

    vec3 ambient = vec3(texture(material.diffuse , texCoords)) * light.ambient;//环境光--
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));
    vec3 glowing = vec3(texture(material.glow, glowPosition));

    //镜面光

    vec3 result = ambient + diffuse + specular + glowing;
    color = vec4(result, 1.0);
}