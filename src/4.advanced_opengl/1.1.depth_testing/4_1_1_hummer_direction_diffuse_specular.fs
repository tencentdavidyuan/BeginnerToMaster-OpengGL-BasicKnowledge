#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shinness;
};

struct Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 WorldPos;
in vec3 WorldNormal;
in vec2 Texcoord;

out vec4 fragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() 
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, Texcoord).rgb;

    // diffuse
    vec3 norm = normalize(WorldNormal);
    vec3 lightDir = normalize(-light.direction);
    float dotNL = dot(norm, lightDir);
    float diff = max(0.0, dotNL);
    vec3 diffuse = light.diffuse * (texture(material.diffuse, Texcoord).rgb * diff);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - WorldPos);
    float dotRV = dot(reflectDir, viewDir);
    float spec = pow(max(0.0, dotRV), material.shinness);
    vec3 specular = light.specular * (texture(material.specular, Texcoord).rgb * spec);

    // fragColor   
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}