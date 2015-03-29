#version 430

uniform sampler2D ParticleTex;
uniform mat4 rotMatrix;

in float Transp;
in float colour;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
	vec2 texCoord = (rotMatrix * vec4(gl_PointCoord, 0, 1)).xy;
    FragColor = texture(ParticleTex, texCoord);
    FragColor.a *= Transp;
	FragColor.r *= colour;
	FragColor.g *= colour;
	FragColor.b = 1.f;
	//FragColor.rgb /= vec3(Transp);
	//FragColor.a = 1.0;
	//FragColor = vec4(1.0,0.0,0.0,1.0);
}
