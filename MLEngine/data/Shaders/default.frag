#version 430 core

in vec3 fragmentColor;
layout (location=0) out vec4 FragColour;

void main()
{
    FragColour = vec4(fragmentColor, 1.0);
}