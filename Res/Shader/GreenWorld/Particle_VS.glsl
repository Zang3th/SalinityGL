#version 450 core

layout(location = 0) in vec2 vertexIn;

out vec2 texCoords;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
    texCoords   = vertexIn + vec2(0.5, 0.5);
    texCoords.y = 1.0 - texCoords.y;

    gl_Position = projection * modelView * vec4(vertexIn, 0.0, 1.0f);
}