#ifndef SCENETYPES_H
#define SCENETYPES_H
#include "GLM\glm.hpp"
#include "..\resources\Shader.h"
#include "..\entities\Camera.h"
#include "..\Messages\PositionMessage.h"
#include "..\Messages\BaseMessage.h"
#include "..\Messages\MessageManager.h"
#include <vector>

struct SceneLight
{
	~SceneLight()
	{
		mauvemessage::MessageManager::ClearMessageListner(this);
	}

	void msg_LightPositionHandler(mauvemessage::BaseMessage* msg)
	{
		mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
		glm::vec3 messagePos = (glm::vec3)*posMsg;
		lightPosition = messagePos;
	}
	glm::vec3 lightPosition;
	glm::vec3 lightIntensity;
	glm::vec3 surfaceReflectivity;
};

struct SceneConfig
{
	std::vector<IEntity*> entities;
	std::vector<CameraEntity*> cameras;
	Shader* currentSceneShader;
	CameraEntity* currentSceneCamera;
	SceneLight* currentSceneLight;
};

#endif