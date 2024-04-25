#version 450 core

layout(location = 0) in vec2 vertexIn;
layout(location = 1) in vec4 colorIn;

uniform mat4 projection;
uniform mat4 model;

out vec3 color;

void main()
{
    float y_offset = float(gl_InstanceID % 100);
    float x_offset = float(gl_InstanceID / 100);

    color = colorIn.xyz;
    color += gl_InstanceID * colorIn.w;

    gl_Position = projection * model * vec4(vertexIn.x + x_offset, vertexIn.y + y_offset, 0.0f, 1.0f);
}