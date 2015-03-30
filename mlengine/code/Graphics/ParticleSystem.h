#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "..\resources\ResourceManager.h"
#include "..\resources\Shader.h"
#include "..\resources\GPUTexture.h"
#include "glm\gtc\constants.hpp"
#include "glm\gtc\matrix_transform.hpp"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Init();

	void Draw(glm::mat4 VP ,glm::vec3 cameraPos);
	void SetPosition(glm::vec3 pos);
	Shader* particleShader;
	bool canDraw;
	void InitParticleBuffers(bool test);
private:

	float randFloat();

	
	ImageTexture* particleTexture;
	GPUTexture* particleGPUTexture;
	GLuint particleRenderSub;
	GLuint particleUpdateSub;

	GLuint posBuf[2], velBuf[2];
	GLuint particleArray[2];
	GLuint feedback[2], initVel, initPos, startTime[2];
	GLuint drawBuf, query;
	int nParticles;
	float angle;
	float particletime, deltaT, graphicdeltaT, oldGraphicTime;
	GLfloat *posArray;
	glm::vec3 currentParticlePosition;
};

#endif