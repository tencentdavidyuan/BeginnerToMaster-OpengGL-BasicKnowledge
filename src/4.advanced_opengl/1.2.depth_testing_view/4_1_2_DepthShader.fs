#version 330 core

in vec2 Texcoord;

out vec4 fragColor;

float linearizeDepth(float depth) 
{
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0;
    return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{
    fragColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1.0);
}