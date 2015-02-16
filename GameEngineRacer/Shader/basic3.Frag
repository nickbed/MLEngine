#version 400

uniform sampler2D tex;
in vec2 texCoord;
in vec3 N;
in vec3 vertPos;

uniform mat3 NormalMatrix;
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ks;			//Specular Reflection
uniform float shininess;
const int MAX_LIGHTS = 10;
uniform int numLights;
uniform struct Light
{
	vec3 position;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float constant;
    float linear;
    float quadratic;
	
}lights[MAX_LIGHTS];



out vec4 FragColour;

vec4 ApplyLight(Light light, vec3 vertPos, vec3 N, vec2 texCoord, sampler2D tex, vec3 Kd, vec3 Ks)
{
	//vec3 lPos = lightPos * light.position;
	vec3 lPos = light.position;
	vec3 L = normalize(lPos - vertPos);
   vec3 R = normalize(2*N* max(dot(N,L),0.0)-L);
   
   //vec3 R = -reflect(L,N);
   vec3 V = normalize(-vertPos);

   //Attenuation
  float distance = length(light.position - vertPos);
  float att = 1.0f / (light.constant + light.linear * distance + 
    			light.quadratic * (distance * distance)); 
   

   vec4 Ia = vec4(Kd,1.0) * (light.ambient ,1.0); // Ambient
   //Ia*=att;
 
   float RV = clamp(dot(R,V),0.0,1.0);
   vec4 Is = vec4(0.0,0.0,0.0,1.0);
 
   Is = (vec4(Ks,1.0)*texture(tex,texCoord)) * pow(RV,shininess);//Specular
	Is*=att;
   vec4 Id = vec4(light.diffuse ,1.0) * max(dot(N,L), 0.0);
   Id = clamp(Id, 0.0, 1.0)*(vec4(Kd,1.0)*texture(tex,texCoord)) ;  //Diffuse   
   Id*=att;

   //Multiply the Reflectivity by the Diffuse intensity
   //return Ia;
   return Id ;//+Is;//+ Ia ;//+ Is;
   //return vec4(light.attenuation,0,0,1);
}

void main() {
    vec3 normal = N;
	normal = normalize( NormalMatrix * normal);
	vec4 finalColour;
   for(int i = 0; i < numLights; i++){
	 finalColour += ApplyLight(lights[i], vertPos, normal, texCoord, tex, Kd, Ks);
		
	}
	FragColour = finalColour;
}
