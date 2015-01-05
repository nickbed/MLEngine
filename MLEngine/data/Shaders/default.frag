#version 330 core

in vec3 lightPos;
in vec3 vertPos; //Vertex position in eye coords
in vec3 N;
in vec3 viewDirection;
in vec2 texCoord; 

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity
uniform sampler2D tex;

layout (location=0) out vec4 FragColour;

void main()
{

	float linearAttenuation = 0.01;
	vec3 intensity = vec3(0.5, 0.5, 0.5);
	vec4 ambientLight = vec4(0.0, 0.0, 0.0, 1.0);
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
   
   //vec4 diffuseLight = vec4(actualFragment, 1.0) + vec4(Kd,1.0) * Id;
   vec4 diffuseLight = vec4(attenuation) * vec4(texture(tex, texCoord).rgb, 1.0) + vec4(Kd,1.0) * Id;
   //vec4 diffuseLight = vec4(attenuation) * vec4(1.0,0.5,0.5, 1.0) + vec4(Kd,1.0) * Id;
   vec4 specularReflection;
   if(dot(N,L) < 0.0)
	{
		specularReflection = vec4(0.0, 0.0, 0.0, 0.0);
	}
   else
	{
		specularReflection = vec4(attenuation) * vec4(pow( max(vec3(0.0), dot( reflect(-L, N), viewDirection ) ), vec3(1.0,1.0,1.0)),1.0);
	}     

   //Multiply the Reflectivity by the Diffuse intensity
   FragColour = (diffuseLight  + ambientLight) * vec4(intensity, 1.0) +  specularReflection;
   //FragColour = texture(tex, texCoord);
   //FragColour = vec4(vec3(gl_FragCoord.z), 1.0f);
}