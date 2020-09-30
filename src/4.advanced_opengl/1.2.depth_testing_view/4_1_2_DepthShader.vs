#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexcoord;

out vec2 Texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Texcoord = aTexcoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}