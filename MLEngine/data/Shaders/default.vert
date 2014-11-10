#version 430 core
uniform mat4 modelmatrix;
uniform mat4 viewprojmatrix;

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec3 colours;

out vec3 fragmentColor;

void main(){
    gl_Position = viewprojmatrix * modelmatrix * vertexPosition_modelspace;
	fragmentColor = colours;
	//fragmentColor = vec3(1.0,1.0,1.0);
}