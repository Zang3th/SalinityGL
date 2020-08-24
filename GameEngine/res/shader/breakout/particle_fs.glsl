#version 330 core

out vec4 FragColor;

uniform vec3 particleColor;

void main()
{
    FragColor = vec4(particleColor, 1.0f);
}