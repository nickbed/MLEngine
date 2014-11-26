#version 330 core
uniform mat4 modelmatrix;
uniform mat4 viewprojmatrix;
uniform mat4 viewmatrix;
uniform vec3 lightposition;

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec3 colours;
layout(location = 2) in vec3 normals;

out vec3 fragmentColor;
out vec3 lightPos;
out vec3 vertPos; //Vertex position in eye coords
out vec3 N; 

void main(){
    gl_Position = viewprojmatrix * modelmatrix * vertexPosition_modelspace;
	lightPos = vec3(viewmatrix * modelmatrix * vec4(lightposition,1.0));
	vertPos = vec3(viewmatrix * modelmatrix * vertexPosition_modelspace);
	N = normalize(normals);	
	fragmentColor = colours;
}