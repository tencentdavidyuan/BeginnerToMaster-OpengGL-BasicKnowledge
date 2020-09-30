#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projectionShadow;

void main() {
    //gl_Position = projection * view * (projectionShadow * (model * vec4(aPos, 1.0)));
    vec4 worldPos = model * vec4(aPos, 1.0);
    vec4 shadowPos = projectionShadow * worldPos;
    gl_Position = projection * view * shadowPos;
}