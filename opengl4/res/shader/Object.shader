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

in vec3 Normal;
in vec3 FragPos;

uniform float specularStrength = 0.5;//镜面反射的度数
uniform vec3 objectColor;
uniform vec3 lightColor; 
uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{

    vec3 norm = normalize(Normal);//法向量的单位向量
    vec3 lightDir = normalize(lightPos - FragPos);//局部空间中物体指向灯光的单位向量

    vec3 viewDir = normalize(viewPos - FragPos);//物体指向摄像机的单位向量
    vec3 reflectDir = reflect(-lightDir, norm);//灯光的镜面反射向量

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
    vec3 specular = specularStrength * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);//计算漫反射强度
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0);
}