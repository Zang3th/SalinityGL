#version 450 core

layout(location = 0) in vec2  vertexIn;
layout(location = 1) in mat4  modelViewIn;
layout(location = 2) in vec4  texOffsetsIn;
layout(location = 3) in float blendFactorIn;

out vec2  texCoords0;
out vec2  texCoords1;
out float blendFactor;

uniform mat4  projection;
uniform float numberOfRows;

void main()
{
    //Calculate texture coordinates
    vec2 texCoords = vertexIn + vec2(0.5, 0.5);
    texCoords.y    = 1.0 - texCoords.y;

    //Set them in relation to the texture atlas
    texCoords /= numberOfRows;

    //Set current and next texture coordinates
    texCoords0 = texCoords + texOffsetsIn.xy;
    texCoords1 = texCoords + texOffsetsIn.zw;

    blendFactor = blendFactorIn;

    gl_Position = projection * modelViewIn * vec4(vertexIn, 0.0, 1.0f);
}