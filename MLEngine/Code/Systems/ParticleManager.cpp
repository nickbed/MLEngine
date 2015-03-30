#include "ParticleManager.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{
	ClearAllParticleSystems();
}

ParticleSystem* ParticleManager::AddNewParticleSystem(glm::vec3 pos, glm::vec3 accel, const char* textureFileName)
{
	ParticleSystem* newPart = new ParticleSystem();
	newPart->InitParticleBuffers(accel, pos, textureFileName);
	ParticleSystems.push_back(newPart);
	return newPart;
}

void ParticleManager::RemoveParticleSystem(ParticleSystem* sysToRemove)
{
	for (int i = 0; i < ParticleSystems.size(); ++i)
	{
		if (ParticleSystems[i] == sysToRemove)
		{
			ParticleSystems.erase(ParticleSystems.begin() + i);
			return;
		}
	}
}

void ParticleManager::ClearAllParticleSystems()
{
	for (auto it = ParticleSystems.begin(); it < ParticleSystems.end(); ++it)
	{
		delete (*it);
	}
	ParticleSystems.clear();
}

void ParticleManager::UpdateAndRenderParticleSystems(float graphicsdt)
{
	float graphicdeltaT = (float)glfwGetTime() - oldGraphicTime;
	//SetPosition(currentParticlePosition);
	oldGraphicTime = (float)glfwGetTime();
	for (auto& i : ParticleSystems)
	{
		i->Draw(VP, cameraPos, graphicdeltaT);
	}
}

void ParticleManager::UpdateManager(glm::mat4 vpIN, glm::vec3 camPosIn)
{
	VP = vpIN;
	cameraPos = camPosIn;
}

