#version 330 core

in vec3 color_out;
in vec2 texCoords_out;
flat in int type_out;

out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
	if (type_out == 1) {
		fragColor = texture(textureSampler, texCoords_out) * vec4(color_out, 1.0);
	}
	else {
		fragColor = vec4(color_out, 1.0);
	}	
}