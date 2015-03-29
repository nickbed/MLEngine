#version 430

uniform sampler2D ParticleTex;

in float Transp;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    FragColor = texture(ParticleTex, gl_PointCoord);
    FragColor.a *= Transp;
	//FragColor.rgb /= vec3(Transp);
	//FragColor.a = 1.0;
	//FragColor = vec4(1.0,0.0,0.0,1.0);
}
