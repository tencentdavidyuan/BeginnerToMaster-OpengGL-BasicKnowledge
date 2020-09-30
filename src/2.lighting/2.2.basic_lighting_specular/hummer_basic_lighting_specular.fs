#version 330 core

in vec3 WorldPos;
in vec3 Normal;
in vec2 Texcoord;

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() 
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float dotNL = dot(norm, lightDir);
    vec3 diffuse = max(0, dotNL) * lightColor;

    // specular
    vec3 reflectLight = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - WorldPos);
    float dotRN = dot(reflectLight, viewDir);
    vec3 specular = pow(max(0, dotRN), 128) * lightColor;

    vec4 tex1 = texture(texture0, Texcoord);
    fragColor = vec4((ambient + diffuse) * tex1.rgb + specular, 1.0);
}