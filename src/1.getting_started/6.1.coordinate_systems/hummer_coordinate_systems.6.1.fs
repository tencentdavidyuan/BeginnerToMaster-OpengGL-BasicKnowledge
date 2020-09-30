#version 330 core

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() 
{
	vec4 tex1 = texture(texture1, Texcoord);
	vec4 tex2 = texture(texture2, Texcoord);

	outColor = mix(tex1, tex2, 0.2);
}