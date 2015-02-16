#version 430

in vec4 vertPosition;
//in vec3 vertNormal;
in vec2 fragTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 texCoord;

void main()
{
	texCoord = fragTexCoord;
	gl_Position = P * V * M * vertPosition;
}
