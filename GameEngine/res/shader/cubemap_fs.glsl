#version 330 core

in vec3 pos_out;

out vec4 fragColor;

uniform samplerCube textureSampler;
uniform vec3 fogColor;

const float lowerLimit = 0.0;
const float upperLimit = 200.0;

void main()
{
	vec4 skyBoxColor = texture(textureSampler, pos_out);
	float factor = (pos_out.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0, 1.0);
	fragColor = mix(vec4(fogColor, 1.0), skyBoxColor, factor);
}