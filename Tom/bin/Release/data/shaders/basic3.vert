#version 330

in vec4 vertPosition;
in vec3 vertNormal;
in vec2 fragTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 LightPosition;

out vec3 vertPos; //Vertex position in eye coords
out vec3 N;
out vec2 texCoord;

void main()
{
	vertPos = vec3( M * vertPosition); //position of vertex (and fragment)
	N = vertNormal;
	texCoord = fragTexCoord;
	gl_Position = P * V * M * vertPosition;
}