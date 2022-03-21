#version 450 core

layout(location = 0) in vec2 vertexIn;

out vec2  texCoords0;
out vec2  texCoords1;
out float blendFactor;

uniform mat4  modelView;
uniform mat4  projection;
uniform vec2  texOffset0;
uniform vec2  texOffset1;
uniform vec2  texCoordInfo; //(numberOfRows, blendFactor)

void main()
{
    //Calculate texture coordinates
    vec2 texCoords = vertexIn + vec2(0.5, 0.5);
    texCoords.y    = 1.0 - texCoords.y;

    //Set them in relation to the texture atlas
    texCoords /= texCoordInfo.x;

    //Set current and next texture coordinates
    texCoords0 = texCoords + texOffset0;
    texCoords1 = texCoords + texOffset1;

    blendFactor = texCoordInfo.y;

    gl_Position = projection * modelView * vec4(vertexIn, 0.0, 1.0f);
}