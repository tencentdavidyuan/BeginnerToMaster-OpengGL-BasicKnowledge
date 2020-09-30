#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexcoord;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	vec4 clipPos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = clipPos;
	//gl_Position = vec4(aPos, 1.0);
	Color = aColor;
	Texcoord = vec2(aTexcoord.x, aTexcoord.y);
}