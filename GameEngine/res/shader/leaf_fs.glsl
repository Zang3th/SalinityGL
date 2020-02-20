#version 330 core

in vec2 texCoords_out;
in float visibility;

out vec4 fragColor;

uniform sampler2D leafTexture;
uniform sampler2D leafMask;
uniform vec3 skyColor;

void main()
{
	vec4 leafColor = texture(leafTexture, texCoords_out);
	vec4 leafMaskColor = texture(leafMask, texCoords_out);
	
	if (leafColor.a < 0.5) {
		discard;
	}

	fragColor = leafColor * leafMaskColor;
	fragColor = mix(vec4(skyColor, 1.0), fragColor, visibility);
}