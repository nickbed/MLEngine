#ifndef SCENETYPES_H
#define SCENETYPES_H
#include "GLM\glm.hpp"
#include "..\resources\Shader.h"
#include "..\entities\Camera.h"
#include "..\Messages\PositionMessage.h"
#include "..\Messages\BaseMessage.h"
#include "..\Messages\MessageManager.h"
#include <vector>
#include <memory>
#include <map>

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
	
	SceneConfig()
	{
		sceneEntities = std::unique_ptr<std::map<std::string, IEntity*>>(new std::map<std::string, IEntity*>);
		sceneCameras = std::unique_ptr<std::map<std::string, CameraEntity*>>(new std::map<std::string, CameraEntity*>);
		sceneLights = std::unique_ptr<std::map<std::string, SceneLight*>>(new std::map<std::string, SceneLight*>);
		stringBin = std::unique_ptr<std::vector<std::string*>>(new std::vector<std::string*>);
	}



	//START Workaround for a VCC bug -_-
	SceneConfig(SceneConfig&& s)
	{
		sceneEntities = std::move(s.sceneEntities);
		sceneCameras = std::move(s.sceneCameras);
		sceneLights = std::move(s.sceneLights);
		stringBin = std::move(s.stringBin);
	}
	//END Workaround for a VCC bug -_-

	//Active in the scene
	std::vector<IEntity*> activeEntities;
	Shader* currentSceneShader;
	CameraEntity* currentSceneCamera;
	SceneLight* currentSceneLight;

	//For storing stuff created by the JSON file
	std::unique_ptr<std::map<std::string, IEntity*>> sceneEntities;
	std::unique_ptr<std::map<std::string, CameraEntity*>> sceneCameras;
	std::unique_ptr<std::map<std::string, SceneLight*>> sceneLights;

	//Bugfix for chars going out of scope
	std::unique_ptr<std::vector<std::string*>> stringBin;
};

#endif