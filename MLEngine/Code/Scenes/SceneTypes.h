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
		sceneEntities = std::unique_ptr<std::map<std::string, IEntity*>>(new std::map<std::string, IEntity*>, std::default_delete<std::map<std::string, IEntity*>>());
		sceneCameras = std::unique_ptr<std::map<std::string, CameraEntity*>>(new std::map<std::string, CameraEntity*>, std::default_delete<std::map<std::string, CameraEntity*>>());
		sceneLights = std::unique_ptr<std::map<std::string, SceneLight*>>(new std::map<std::string, SceneLight*>, std::default_delete<std::map<std::string, SceneLight*>>());
		numActiveEntities = 0;
		numActiveLights = 0;
		coin =0;
		currentPlayer = nullptr;
		hasWaterfall = false;
	}

	~SceneConfig()
	{
		for (int i = 0; i < numActiveEntities; ++i)
		{
			activeEntities[i]->Destroy();
		}

		std::map<std::string, IEntity*>::iterator startit = sceneEntities->begin();
		for(auto it = sceneEntities->begin(); it != sceneEntities->end(); ++it)
		{
			if(it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
		}
		sceneEntities->clear();

		//for(auto it = sceneCameras->begin(); it != sceneCameras->end(); ++it)
		//{
		//	if(it->second != nullptr)
		//	{
		//		delete it->second;
		//	}
		//}
		//sceneCameras->clear();

		//for(auto it = sceneLights->begin(); it != sceneLights->end(); ++it)
		//{
		//	if(it->second != nullptr)
		//	{
		//		delete it->second;
		//	}
		//}
		//sceneLights->clear();

	}


	//START Workaround for a VCC bug -_-
	SceneConfig(SceneConfig&& s)
	{
		sceneEntities = std::move(s.sceneEntities);
		sceneCameras = std::move(s.sceneCameras);
		sceneLights = std::move(s.sceneLights);
		currentPlayer = std::move(s.currentPlayer);
	}
	//END Workaround for a VCC bug -_-

	//Active in the scene
	//std::vector<IEntity*> activeEntities;
	IEntity* activeEntities[2000];
	int coin;
	int numActiveEntities;
	SceneLight activeLights[128];
	int numActiveLights;
	Shader* currentSceneShader;
	CameraEntity* currentSceneCamera;
	IEntity* currentPlayer;
	bool hasWaterfall;
	//SceneLight* currentSceneLight;

	//For storing stuff created by the JSON file
	std::unique_ptr<std::map<std::string, IEntity*>> sceneEntities;
	std::unique_ptr<std::map<std::string, CameraEntity*>> sceneCameras;
	std::unique_ptr<std::map<std::string, SceneLight*>> sceneLights;
	std::string filename;
	
};

#endif