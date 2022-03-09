#version 450 core

in vec2 texCoords;
in vec3 fragPos;
in vec4 clipSpace;
in vec3 toCameraVector;

out vec4 fragColor;

uniform float     moveFactor;
uniform float     nearPlane;
uniform float     farPlane;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform vec3      viewPos;
uniform vec3      lightPos;
uniform vec3      lightColor;

const float waveStrength = 0.005;
const vec4  waterColor = vec4(0.0, 0.2, 0.8, 1.0);
const float fresnelReflectivness = 0.2;
const float shininess = 100.0;
const float specularStrength = 0.5;

void main()
{
    //Convert to normalized device space
    vec2 normalizedDeviceSpace  = ((clipSpace.xy / clipSpace.w) / 2.0) + 0.5;
    vec2 reflectTexCoords       = vec2(normalizedDeviceSpace.x, -normalizedDeviceSpace.y);
    vec2 refractTexCoords       = vec2(normalizedDeviceSpace.x, normalizedDeviceSpace.y);

    //Sample from refraction depth texture
    float depth = texture(depthMap, refractTexCoords).r;

    //Distance of the camera to the floor
    float floorDistance = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));

    //Distance of the camera to the water surface
    depth               = gl_FragCoord.z;
    float waterDistance = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));

    //Calculate water depth at given fragment
    float waterDepth = floorDistance - waterDistance;

    //Sample distortion value from DuDvMap
    vec2 distortedTexCoords = (texture(dudvMap, vec2(texCoords.x + moveFactor, texCoords.y)).rg) * 0.1;
    distortedTexCoords      = texCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion    = ((texture(dudvMap, distortedTexCoords).rg) * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 3.0, 0.0, 1.0);

    //Add distortion to the reflection and refraction texture
    reflectTexCoords += totalDistortion;
    refractTexCoords += totalDistortion;

    //Clamp texture coordinates to prevent wrapping
    reflectTexCoords.x  = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y  = clamp(reflectTexCoords.y, -0.999, -0.001);
    refractTexCoords    = clamp(refractTexCoords, 0.001, 0.999);

    //Sample from reflect/refract textures at calculated coordinates
    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);

    //Sample from normal map
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal         = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 4.0, normalMapColor.g * 2.0 - 1.0);
    normal              = normalize(normal);

    //Fresnel effect
    vec3 viewVector         = normalize(toCameraVector);
    float refractiveFactor  = dot(viewVector, normal);
    refractiveFactor        = pow(refractiveFactor, fresnelReflectivness);
    refractiveFactor        = clamp(refractiveFactor, 0.0, 1.0);

    //Calculate specular light
    vec3 lightDir   = normalize(lightPos - fragPos);
    vec3 viewDir    = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular   = lightColor * spec * specularStrength * clamp(waterDepth / 3.0, 0.0, 1.0);

    vec4 outColor   = mix(reflectColor, refractColor, refractiveFactor);
    fragColor       = mix(outColor, waterColor, 0.2) + vec4(specular, 0.0);

    //Add soft edges
    fragColor.a = clamp(waterDepth / 1.0, 0.0, 1.0);
}