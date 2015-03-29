#version 440
uniform mat4 modelmatrix;
uniform mat4 viewprojmatrix;

layout(location = 0) in vec4 vertexPosition_modelspace;

out vec3 skyboxCoord; 

void main(){
    gl_Position = (viewprojmatrix * vertexPosition_modelspace);
	skyboxCoord = vec3(vertexPosition_modelspace);
}