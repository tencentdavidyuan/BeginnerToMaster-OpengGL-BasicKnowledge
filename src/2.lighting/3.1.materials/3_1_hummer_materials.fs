#version 330 core

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shinness;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 WorldPos;
in vec3 WorldNormal;
in vec2 Texcoord;

out vec4 fragColor;

uniform vec3 lightColor;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform sampler2D texture1;
uniform sampler2D texture2;


void main() 
{
    // tex1
    vec4 tex1 = texture2D(texture1, Texcoord);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3  lightDir = normalize(light.position - WorldPos);
    vec3  norm = normalize(WorldNormal);
    float dotNL = dot(lightDir, norm);
    float diff = max(0.0, dotNL);
    vec3  diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3  reflectDir = reflect(-lightDir, norm);
    vec3  viewDir = normalize(viewPos - WorldPos);
    float dotRV = dot(reflectDir, viewDir);
    float spec = pow(max(0.0, dotRV), material.shinness);
    vec3 specular = light.specular * (spec * material.specular);

    //fragColor = vec4((ambient + diffuse) * vec3(tex1) + specular, 1.0);
    //fragColor = vec4(diffuse * vec3(tex1), 1.0);
    fragColor = vec4((ambient + diffuse) * vec3(tex1) + specular , 1.0);
}