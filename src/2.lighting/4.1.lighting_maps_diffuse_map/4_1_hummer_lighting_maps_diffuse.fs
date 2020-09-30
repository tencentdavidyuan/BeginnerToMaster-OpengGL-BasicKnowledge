#version 330 core 

struct Material
{
    sampler2D diffuse;

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

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() 
{
    vec3 diffuseTexture = vec3(texture(material.diffuse, Texcoord));

    // ambient
    vec3 ambient = light.ambient * diffuseTexture;

    // diffuse
    vec3 norm = normalize(WorldNormal);
    vec3 lightDir = normalize(light.position - WorldPos);
    float dotNL = dot(lightDir, norm);
    float diff = max(dotNL, 0.0);    
    vec3 diffuse = light.diffuse * (diff * diffuseTexture);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - WorldPos);
    float dotVR = dot(reflectDir, viewDir);
    float spec = pow(max(dotVR, 0.0), material.shinness);
    vec3 specular = light.specular * (spec * material.specular);

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
