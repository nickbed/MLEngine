#version 330 core

in vec3 lightPos;
in vec3 vertPos; //Vertex position in eye coords
in vec3 N;
in vec3 viewDirection;
in vec2 texCoord;
in vec3 skyboxCoord; 

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity
uniform vec3 Sd;			// Specular intensity
uniform float Sp;			// Specular power

uniform vec3 cameraPos;
uniform sampler2D tex;
uniform samplerCube gCubemapTexture;
uniform vec3 isskybox;

layout (location=0) out vec4 FragColour;

void main()
{

	float linearAttenuation = 0.06;
	vec3 intensity = vec3(0.5, 0.5, 0.5);
	vec4 ambientLight = vec4(0.1, 0.1, 0.1, 0.0);
	vec4 ambient = ambientLight;
	//Calculate the light vector
   float vertToLDistance = length(lightPos - vertPos);
   vec3 L = normalize(lightPos - vertPos);
   
   //Attenuation
   float attenuation = 1.0/ (linearAttenuation * vertToLDistance); 
   //float attenuation = 1.0; 
     
   //calculate Diffuse Light Intensity making sure it is not negative 
   //and is clamped 0 to 1  
   vec4 Id = vec4(Ld,1.0) * max(dot(N,L), 0.0);
   Id = clamp(Id, 0.0, 1.0);
   
   vec4 diffuseLight = vec4(vec3(attenuation), 1.0) * texture(tex, texCoord) + vec4(Kd,0.0) * Id;

   //calulate Specular light
   vec3 viewVector = normalize(cameraPos - vertPos);
   vec3 reflectionVector = normalize(reflect(-lightPos, N));
   float specular = max(dot(viewVector, reflectionVector), 0.0);
   specular = pow(specular, (Sp)); 
   specular = clamp(specular, 0.0, 1.0);
   vec3 finalSpec = vec3(vec3(Sd) * specular);
   finalSpec = clamp(finalSpec, 0.0, 1.0);
   if(dot(N, lightPos) < 0.0)
   {
		finalSpec = vec3(0.0);
   }  

   //Multiply the Reflectivity by the Diffuse intensity
   //FragColour = (diffuseLight  + ambientLight + vec4(finalSpec, 1.0)) * vec4(intensity, 1.0) ;
   FragColour = diffuseLight + vec4(finalSpec, 0.0);
  // FragColour = vec4(finalSpec, 1.0);	 
   //FragColour = texture(tex, texCoord);
   //FragColour = vec4(vec3(gl_FragCoord.z), 1.0f);
}