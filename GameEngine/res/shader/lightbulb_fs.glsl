#version 330 core

in vec2 texCoords_out;
in float visibility;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec3 fogColor;
uniform vec3 lightColor;

void main()
{
	//Texturen
	vec4 texColor = texture(textureSampler, texCoords_out) * vec4(lightColor, 1.0);

	//Fog
	vec3 newFogColor = fogColor * 0.6;
	fragColor = mix(vec4(newFogColor, 1.0), texColor, visibility);
}