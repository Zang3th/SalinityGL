#version 450 core

in vec2  texCoords0;
in vec2  texCoords1;
in float blendFactor;

out vec4 fragColor;

uniform sampler2D textureAtlas;

void main()
{
    vec4 color0 = texture(textureAtlas, texCoords0);
    vec4 color1 = texture(textureAtlas, texCoords1);

    fragColor = mix(color0, color1, blendFactor);
}