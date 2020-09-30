#version 330 core

in VSOutput
{
    vec3 WorldPos;
    vec2 Texcoord;
    vec3 WorldNormal; 
} fs_in;

out vec4 fragColor;

uniform sampler2D diffuseMap;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec3 normal = normalize(fs_in.WorldNormal);

    // ambient
    vec3 ambient = (lightColor * 0.1) * texture(diffuseMap, fs_in.Texcoord).rgb;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.WorldPos);
    float diff = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse = lightColor * (diff * texture(diffuseMap, fs_in.Texcoord).rgb);
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, -halfwayDir), 0.0), 32.0);
    vec3 specular = (lightColor * 0.2) * spec;

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}