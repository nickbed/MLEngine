#version 400

in vec3 vertPos;
in vec3 texcoords;
in vec3 N;
in vec3 lightPos;

uniform samplerCube cube_texture;
uniform mat4 V; // view matrix

out vec4 FragColour;

uniform float ReflectFactor; // Amount of reflection

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity

uniform vec3 Ks;			//Specular Reflection

uniform vec3 Ka;			// Ambient reflectivity
uniform vec3 La;			// Ambient Light Intensity

uniform float n;   //Specular exponent
uniform float f;	//Fall off
uniform float fsize;	//Fall off size of area



uniform bool frag;


void main () {

  //normalizes the eye vector.
  vec3 incident_eye = normalize (vertPos);

  // normalizes the normal vector
  vec3 normal = normalize (N);

  //reflected vector of the eye.
  vec3 reflected = normalize(reflect (incident_eye, normal));

  reflected = vec3 (inverse(V) * vec4 (reflected, 0.0));

  vec4 cubeMapColour = texture (cube_texture, reflected);
 
  // Light vector
  vec3 L = normalize(lightPos - vertPos);
  //Reflect vector
  vec3 R = normalize(2*N* max(dot(N,L),0.0)-L);
  // View Vector
  vec3 V = normalize(-vertPos);

  //Attenuation
  float att = 1- pow((length(lightPos - vertPos)/fsize),f);

  // Ambient
  vec4 Ia = vec4(Ka,1.0) * vec4(La,1.0);
  float RV = clamp(dot(R,V),0.0,1.0);

  //  Specular
  vec4 Is = vec4(0.0,0.0,0.0,1.0);
  Is = att * vec4(Ks,1.0) * pow(RV,n);

  //Diffuse
  vec4 Id = att * vec4(Ld,1.0) * max(dot(N,L), 0.0);
  Id = clamp(Id, 0.0, 1.0)*vec4(Kd,1.0) ;     

  //Diffuse + Spec + Ambient
  vec4 phong = Ia + Id + Is;


 if(frag == true)
 {
	FragColour = phong* cubeMapColour* ReflectFactor ;
	//FragColour = mix(phong, cubeMapColour, ReflectFactor) ;
 }
 else {
	FragColour = texture (cube_texture, texcoords);
 }
  
  
  
}