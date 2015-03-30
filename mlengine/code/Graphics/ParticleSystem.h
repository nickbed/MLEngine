#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "..\resources\ResourceManager.h"
#include "..\resources\Shader.h"
#include "..\resources\GPUTexture.h"
#include "glm\gtc\constants.hpp"
#include "GLM\gtx\transform.hpp"


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Init();

	void Draw(glm::mat4 VP ,glm::vec3 cameraPos, float dt);
	void LoadTexture(const char* filename);

	void SetPosition(glm::vec3 pos, bool setPosition);
	void SetAcceleration(glm::vec3 accel, bool randomise);
	void SetLifetime(float lifetime);
	void SetUseRandomPositionRange(bool enable, float minX, float minZ, float maxX, float maxZ);
	void SetParticleSize(float size);

	Shader* particleShader;
	bool canDraw;
	void InitParticleBuffers(glm::vec3 accel, glm::vec3 position, const char* texturename);
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
	float pSize;
	GLfloat *posArray;
	GLfloat *accelArray;
	glm::vec3 currentParticlePosition;
	glm::vec3 currentParticleAcceleration;
	float currentLifetime;
	bool useRandomPositionRange;
	float randPosRangeXmin, randPosRangeZmin, randPosRangeXmax, randPosRangeZmax;
};

#endif