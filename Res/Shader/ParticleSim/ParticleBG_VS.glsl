#version 450 core

layout(location = 0) in vec4 vertexIn;

out vec2 screenPos;

uniform mat4 projection;

void main()
{
    vec4 pos = vec4(vertexIn.x * 1920.0, vertexIn.y * 1080.0, 0.0, 1.0);
    screenPos = pos.xy;

    gl_Position = projection * pos;
}