#version 440

in vec2 UV;

out vec4 color;

uniform sampler2D textureUnit;

void main(){

	color = texture2D( textureUnit, UV );
	
	
}