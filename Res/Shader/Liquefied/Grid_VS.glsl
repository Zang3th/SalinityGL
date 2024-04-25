#version 450 core

layout(location = 0) in vec2 vertexIn;

uniform mat4 projection;
uniform mat4 model;

out vec3 color;

void main()
{
    float x_offset = float(gl_InstanceID % 150);
    float y_offset = float(gl_InstanceID / 150);

    color = vec3(0.1f, 0.1f, 0.1f);
    color += gl_InstanceID * 0.000025;

    gl_Position = projection * model * vec4(vertexIn.x + x_offset, vertexIn.y + y_offset, 0.0f, 1.0f);
}