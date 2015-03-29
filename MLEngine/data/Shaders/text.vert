#version 440

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;

uniform vec2 screenSize;
out vec2 UV;

void main(){
	vec2 halfScreenSize;
	halfScreenSize.x = screenSize.x /2;
	halfScreenSize.y = screenSize.y /2;
	vec2 vertexPosition_homoneneousspace = vertexPosition - halfScreenSize;
	vertexPosition_homoneneousspace /= halfScreenSize;
	gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
	UV = vertexUV;
}

