#version 330 core
uniform mat4 modelmatrix;
uniform mat4 viewprojmatrix;
uniform mat3 normalmatrix;
uniform mat4 viewmatrix;
uniform vec3 lightposition;

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 fragTexCoord;


out vec3 lightPos;
out vec3 vertPos; //Vertex position in eye coords
out vec3 viewDirection;
out vec3 N;
out vec2 texCoord; 

void main(){
    gl_Position = viewprojmatrix * modelmatrix * vertexPosition_modelspace;
	lightPos = vec3(viewmatrix * vec4(lightposition,1.0));
	vertPos = vec3(viewmatrix * modelmatrix * vertexPosition_modelspace);
	N = normalize(normalmatrix * normals);
	viewDirection = vec3(normalize(-vertexPosition_modelspace));	
	texCoord = fragTexCoord;
}