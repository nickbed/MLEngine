#version 330 core

in vec3 fragmentColor;
in vec3 lightPos;
in vec3 vertPos; //Vertex position in eye coords
in vec3 N; 

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity

layout (location=0) out vec4 FragColour;

void main()
{
	
	//Calculate the light vector
   vec3 L = normalize(lightPos - vertPos);  
    
   //calculate Diffuse Light Intensity making sure it is not negative 
   //and is clamped 0 to 1  
   vec4 Id = vec4(Ld,1.0) * max(dot(N,L), 0.0);
   Id = clamp(Id, 0.0, 1.0);     

   //Multiply the Reflectivity by the Diffuse intensity
   FragColour = vec4(fragmentColor,1.0) * vec4(Kd,1.0) * Id;
   //FragColour = vec4(Kd,1.0) * Id;
   //FragColour = vec4(vec3(gl_FragCoord.z), 1.0f);
}