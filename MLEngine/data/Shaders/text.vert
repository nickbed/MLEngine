#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;
uniform vec2 screenSize;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	vec2 halfScreenSize;
	halfScreenSize.x = screenSize.x /2;
	halfScreenSize.y = screenSize.y /2;
	vec2 vertexPosition_homoneneousspace = vertexPosition_screenspace - halfScreenSize;
	vertexPosition_homoneneousspace /= halfScreenSize;
	gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

