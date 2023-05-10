
//#shader fragment

#version 330 core

out vec4 color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    //sampler2D glow;
    float shininess;
};

struct SpotLight {

    vec3  position;
    vec3  direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//垂直扩散的参数
    float linea;
    float quadratic;

    float cutOff;//内外圆锥角度取cos
    float outerCutOff;
};

struct DirLight {

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//垂直扩散的参数
    float linea;
    float quadratic;
};

#define NR_POINT_LIGHTS 4

uniform PointLight pointlight[NR_POINT_LIGHTS];
uniform DirLight dirlight;
uniform SpotLight spotlight;

uniform Material material;
uniform vec3 viewPos;
//uniform float movement;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoords;

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal , lightDir) , 0.0);
    vec3 reflectDir = reflect(-lightDir , normal);
    float spec = pow(max(dot(viewDir , reflectDir) , 0.0) ,material.shininess);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse , texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular , texCoords));
    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
    vec3 lightDir = normalize(light.position - FragPos);    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linea * distance
        + light.quadratic * (distance * distance));

    float diff = max(dot(normal , lightDir) , 0.0);
    vec3 reflectDir = reflect(-lightDir , normal);
    float spec = pow(max(dot(viewDir , reflectDir) , 0.0) , material.shininess);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse , texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular , texCoords));
    return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight light , vec3 normal , vec3 viewDir , vec3 FragPos)
{    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linea * distance
        + light.quadratic * (distance * distance));
    vec3 lightDir = normalize(light.position - FragPos);//局部空间中物体指向灯光的单位向量

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(lightDir , normal) , 0);
    vec3 reflectDir = reflect(-lightDir , normal);
    float spec = pow(max(dot(viewDir , reflectDir) , 0.0) , material.shininess);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse ,texCoords));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse ,texCoords));
    vec3 specular = light.specular * vec3(texture(material.diffuse ,texCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;//垂直衰减

    diffuse *= intensity;
    specular *= intensity;//水平衰减
    return ambient + diffuse + specular;
}
void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);//法向量的单位向量
    vec3 output;
    output += CalculateDirectionalLight(dirlight , norm , viewDir);
    for (int i = 0; i < 4; i++)
    {
        output += CalculatePointLight(pointlight[i] , norm , viewDir , FragPos);
    }
    output += CalculateSpotLight(spotlight , norm , viewDir , FragPos);

    color = vec4(output , 1.0);
}