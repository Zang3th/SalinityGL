#version 450 core

in vec2 texCoords;
in vec4 fragPos;
in vec4 clipSpace;

out vec4 fragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main()
{
    //Convert to normalized device space
    vec2 normalizedDeviceSpace = ((clipSpace.xy / clipSpace.w) / 2.0) + 0.5;
    vec2 reflectTexCoords = vec2(normalizedDeviceSpace.x, -normalizedDeviceSpace.y);
    vec2 refractTexCoords = vec2(normalizedDeviceSpace.x, normalizedDeviceSpace.y);

    //Sample from reflect/refract textures at calculated coordinates
    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);

    fragColor = mix(reflectColor, refractColor, 0.5f);
}