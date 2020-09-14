#version 440 core

in vec2 TexOut;
in vec3 ColorOut;
out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    FragColor = vec4(ColorOut, 1.0f) * texture(tex, TexOut);
}