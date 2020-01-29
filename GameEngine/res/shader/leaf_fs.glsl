#version 330 core

in vec3 color_out;
in vec2 texCoords_out;

out vec4 fragColor;

uniform sampler2D leafTexture;
uniform sampler2D leafMask;

void main()
{
	vec4 leafColor = texture(leafTexture, texCoords_out);
	vec4 leafMaskColor = texture(leafMask, texCoords_out);
	
	if (leafColor.a < 0.5) {
		discard;
	}

	fragColor = leafColor * leafMaskColor;;
}