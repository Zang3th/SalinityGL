#version 450 core

in vec2 texCoords;
in vec3 normals;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D alphaMask;
uniform int gotAlphaMask;
uniform vec3 viewPos;

void main()
{
    vec4 color = texture(diffuseTexture, texCoords);
    float alphaValue = 1.0;

    //Check for alpha mask
    if(gotAlphaMask == 1)
    {
        vec4 mask  = texture(alphaMask, texCoords);
        alphaValue = mask.r + mask.g + mask.b;
    }

    fragColor = vec4(color.rgb, alphaValue);

    if(fragColor.a < 0.5)
        discard;
}