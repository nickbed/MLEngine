#version 400

layout (location=0) in vec4 vertPosition;
layout (location=1) in vec3 vertNormal;
layout (location=2) in vec2 fragTexCoord;

uniform vec3 LightPosition; // Light position 
uniform mat3 NormalMatrix;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 texcoords;
out vec3 vertPos; //Vertex position in eye coords
out vec3 N; //Transformed normal
out vec3 lightPos; //Light position in eye coords

void main () {
  texcoords = vertPosition;
  vertPos = vec3(V * M * vec4(vp,1.0)); // Vertex position
  lightPos = vec3(V * M * vec4(LightPosition,1.0)); //Light Position
  N = normalize( NormalMatrix * vertNormal);  //Normal
  gl_Position = P * V * M * vec4 (vertPosition, 1.0); //Current vertex position
}

