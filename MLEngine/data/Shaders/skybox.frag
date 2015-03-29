#version 440

in vec3 skyboxCoord; 

uniform samplerCube gCubemapTexture;

layout (location=0) out vec4 FragColour;

void main()
{
   FragColour = texture(gCubemapTexture, skyboxCoord);
}