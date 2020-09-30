#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 Texcoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(0, dot(norm, lightDir));
	vec3 diffuse = diff * lightColor;

	// texture mix
	vec3 tex1 = texture(texture1, Texcoord);
	vec3 tex2 = texture(texture2, Texcoord);
	vec3 texMix = mix(tex1, tex2, 0.2f) * objectColor;

	fragColor = vec4(texMix * diffuse + ambient, 1.0);

}