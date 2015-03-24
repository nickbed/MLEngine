#version 430

in vec3 vertPos;
in vec3 N;
in vec3 lightPos;
in vec3 texcoords;

uniform sampler2D tex;
in vec2 texCoord;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity

uniform vec3 Ks;			//Specular Reflection

uniform vec3 Ka;			// Ambient reflectivity
uniform vec3 La;			// Ambient Light Intensity

uniform float n;   //Specular exponent
uniform float f;	//Fall off
uniform float fsize;	//Fall off

out vec4 FragColour;

void main() {

   vec3 L = normalize(lightPos - vertPos);
   vec3 R = normalize(2*N* max(dot(N,L),0.0)-L);
   
   //vec3 R = -reflect(L,N);
   vec3 V = normalize(-vertPos);
   float att = 1- pow((length(lightPos - vertPos)/fsize),f);
   
   
   vec4 Ia = vec4(Ka,1.0) * vec4(La,1.0);

   float RV = clamp(dot(R,V),0.0,1.0);
   vec4 Is = vec4(0.0,0.0,0.0,1.0);

   Is = att* vec4(Ks,1.0) * pow(RV,n);

   vec4 Id = att* vec4(Ld,1.0) * max(dot(N,L), 0.0);
   Id = clamp(Id, 0.0, 1.0)*vec4(Kd,1.0) ;     

   //Multiply the Reflectivity by the Diffuse intensity
   FragColour = Id + Ia + Is;
   //FragColour = texture(tex,texCoord);
}

   