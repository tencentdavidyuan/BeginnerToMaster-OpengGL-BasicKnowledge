#version 330 core

in vec2 Texcoord;

out vec4 fragColor;

uniform sampler2D planeMap;

void main()
{
    fragColor = texture(planeMap, Texcoord);
}