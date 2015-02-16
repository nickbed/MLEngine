#version 430

layout (location=0) in vec3 vertPosition;
layout (location=1) in vec3 vertNormal;
layout (location=2) in vec2 fragTexCoord;

out vec3 vertPos; //Vertex position in eye coords
out vec3 N; //Transformed normal
out vec3 lightPos; //Light position in eye coords
out vec2 texCoord;

uniform vec3 LightPosition; // Light position 
uniform mat3 NormalMatrix;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	texCoord = fragTexCoord;
   vertPos = vec3(V * M * vec4(vertPosition,1.0)); 
   lightPos = vec3(V * M * vec4(LightPosition,1.0)); 
   N = normalize( NormalMatrix * vertNormal);  
   gl_Position = P * V * M * vec4 (vertPosition, 1.0);
}

