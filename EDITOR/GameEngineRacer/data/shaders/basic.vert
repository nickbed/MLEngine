#version 430

in vec4 vertPosition;
in vec3 vertColour;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProjection;


out vec3 colour;

void main()
{
	colour = vertColour;
	gl_Position = mProjection * mView * mModel * vertPosition;
	
	
}