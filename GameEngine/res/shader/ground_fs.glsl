#version 330 core

in vec3 heightcolor_out;
in vec2 texCoords_out;
in vec2 blendmapCoords_out;
in float visibility;

out vec4 fragColor;

uniform sampler2D grassTexture;
uniform sampler2D dirtTexture;
uniform sampler2D stoneTexture;
uniform sampler2D blendmap;
uniform vec3 skyColor;

void main()
{
	//Texturen (Grass, Feldweg)
	vec4 blendmapColor = texture(blendmap, blendmapCoords_out);
	vec4 rTexture = texture(dirtTexture, texCoords_out) * blendmapColor.r * 0.5;
	vec4 gTexture = texture(grassTexture, texCoords_out) * blendmapColor.g * vec4(heightcolor_out, 1.0);
	vec4 bTexture = texture(stoneTexture, texCoords_out) * blendmapColor.b;
	vec4 groundColor = (rTexture + gTexture + bTexture);

	//Fog
	fragColor = mix(vec4(skyColor, 1.0), groundColor, visibility);
}