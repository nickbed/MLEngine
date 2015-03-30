#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include "..\Graphics\ParticleSystem.h"
#include <vector>

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	ParticleSystem* AddNewParticleSystem(glm::vec3 pos, glm::vec3 accel, const char* textureFileName);
	void RemoveParticleSystem(ParticleSystem* sysToRemove);
	void ClearAllParticleSystems();
	void UpdateAndRenderParticleSystems(float graphicsdt);
	void UpdateManager(glm::mat4 VP, glm::vec3 cameraPos);


private:
	std::vector<ParticleSystem*> ParticleSystems;
	glm::mat4 VP;
	glm::vec3 cameraPos;
	float oldGraphicTime;
};

#endif