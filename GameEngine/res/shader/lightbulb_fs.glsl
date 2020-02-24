#version 330 core

in vec2 texCoords_out;
in float visibility;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec3 skyColor;
uniform vec3 lightColor;

void main()
{
	//Texturen
	vec4 texColor = texture(textureSampler, texCoords_out) * vec4(lightColor, 1.0);

	//Fog
	fragColor = mix(vec4(skyColor, 1.0), texColor, visibility);
}