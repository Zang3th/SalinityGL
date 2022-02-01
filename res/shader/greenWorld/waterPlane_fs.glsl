#version 450 core

in vec2 texCoords;
in vec4 fragPos;
in vec4 clipSpace;

out vec4 fragColor;

uniform float     moveFactor;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D DuDvMap;

const float waveStrength = 0.01;
const vec4  waterColor   = vec4(0.0f, 0.2f, 0.7f, 1.0f);

void main()
{
    //Convert to normalized device space
    vec2 normalizedDeviceSpace = ((clipSpace.xy / clipSpace.w) / 2.0) + 0.5;
    vec2 reflectTexCoords = vec2(normalizedDeviceSpace.x, -normalizedDeviceSpace.y);
    vec2 refractTexCoords = vec2(normalizedDeviceSpace.x, normalizedDeviceSpace.y);

    //Sample distortion value from DuDvMap
    vec2 distortedTexCoords1 = ((texture(DuDvMap, vec2(texCoords.x + moveFactor, texCoords.y)).rg * 2.0) - 1.0) * waveStrength;
    vec2 distortedTexCoords2 = ((texture(DuDvMap, vec2(-texCoords.x + moveFactor, texCoords.y) + moveFactor).rg * 2.0) - 1.0) * waveStrength;
    vec2 totalDistortion = distortedTexCoords1 + distortedTexCoords2;

    //Add distortion to the reflection and refraction texture
    reflectTexCoords += totalDistortion;
    refractTexCoords += totalDistortion;

    //Clamp texture coordinates to prevent wrapping
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    //Sample from reflect/refract textures at calculated coordinates
    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);

    vec4 outColor = mix(reflectColor, refractColor, 0.5f);
    fragColor = mix(outColor, waterColor, 0.2f);
}