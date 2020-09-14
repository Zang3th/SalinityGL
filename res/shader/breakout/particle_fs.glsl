#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 particleColor;

void main()
{
    FragColor = vec4(particleColor, 1.0f) * texture(tex, TexCoords);
}