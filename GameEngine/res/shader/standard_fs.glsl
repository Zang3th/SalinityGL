#version 330 core

in vec3 color_out;
in vec2 texCoords_out;

out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
	fragColor = texture(textureSampler, texCoords_out) * vec4(color_out, 1.0);
}