#version 330 core

in vec2 Texcoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 fragColor;

void main() 
{
	vec4 tex1 = texture(texture1, Texcoord);
	vec4 tex2 = texture(texture2, Texcoord);

	vec4 mixTex = mix(tex1, tex2, 0.2f);
	fragColor = vec4((mixTex.rgb * objectColor * lightColor), 1.0) * 2.0;
}
