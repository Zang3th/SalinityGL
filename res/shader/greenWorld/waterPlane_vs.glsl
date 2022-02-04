#version 450 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 texCoordsIn;

out vec2 texCoords;
out vec3 fragPos;
out vec4 clipSpace;
out vec3 toCameraVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

const float tileDivisor = 128.0f;

void main()
{
    texCoords = texCoordsIn / tileDivisor;
    vec4 worldPos = model * vec4(vertexIn, 1.0f);
    fragPos = worldPos.xyz;
    clipSpace = projection * view * worldPos;
    toCameraVector = viewPos - fragPos.xyz;

    gl_Position = clipSpace;
}