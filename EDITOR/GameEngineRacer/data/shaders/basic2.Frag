#version 430

uniform sampler2D tex;
in vec2 texCoord;


out vec4 FragColour;

void main() {
   
   FragColour = texture(tex,texCoord);
}