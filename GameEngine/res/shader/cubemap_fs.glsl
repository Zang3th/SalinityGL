#version 330 core

in vec3 pos_out;

out vec4 fragColor;

uniform samplerCube textureSampler;

void main()
{
	fragColor = texture(textureSampler, pos_out);
}