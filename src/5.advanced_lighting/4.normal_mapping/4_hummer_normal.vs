#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VSOutput
{
    vec3 WorldPos;
    vec2 Texcoord;
    vec3 WorldNormal;    

} vsOutput;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vsOutput.WorldPos = (model * vec4(aPos, 1)).xyz;
    vsOutput.Texcoord = aTexcoord;
    vsOutput.WorldNormal = transpose(inverse(mat3(model))) * aNormal;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}