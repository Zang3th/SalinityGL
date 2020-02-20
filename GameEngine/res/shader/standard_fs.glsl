#version 330 core

in vec2 texCoords_out;
in float visibility;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec3 skyColor;

void main()
{
	fragColor = texture(textureSampler, texCoords_out);
	fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
}