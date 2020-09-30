#version 330 core

in VSOutput
{
    vec3 WorldPos;
    vec2 Texcoord;
    
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentPos;
} fs_in;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightColor;

void main()
{
    // obtain normal from the normal map
    // this normal is in tangent space
    vec3 normal = texture(normalMap, fs_in.Texcoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // ambient 
    vec3 ambient = (lightColor * 0.1) * texture(diffuseMap, fs_in.Texcoord).rgb;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentPos);
    float dotNL = dot(normal, lightDir);
    float diff = max(0.0, dotNL);
    vec3 diffuse = lightColor * (diff * texture(diffuseMap, fs_in.Texcoord).rgb);

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float dotVH = dot(normal, halfDir);
    float spec = pow(max(0.0, dotVH), 16.0);
    vec3 specular = (lightColor * 0.2) * spec;

    // result
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}