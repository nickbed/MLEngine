#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "..\Assert\Assert.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Interfaces\IEntity.h"
#include "..\resources\Shader.h"
#include "..\entities\Camera.h"
#include "..\entities\GeneralEntity.h"
#include "..\Components\BasicKeyMovement.h"
#include "..\Components\MousePoller.h"
#include "..\Components\threeDGraphics.h"
#include "..\Messages\BaseMessage.h"
#include "SceneTypes.h"
#include <vector>





class SceneManager
{
public:
	SceneManager(std::unique_ptr<GraphicsManager> graph);

	//Load scene into engine
	bool LoadScene(SceneConfig scene);

	//Generate scene from file
	SceneConfig LoadSceneFromFile(const char* fileName);

	//Do manager Init stuff
	bool InitSceneManager();

	//Init current stored scene and it's entities
	bool InitCurrentScene();

	//Update all entities
	bool UpdateCurrentSceneEntities(float dt);

	//Draw all scene entities
	bool DrawCurrentSceneEntities(float dt);

	//Destroy all scene entities and their components
	bool DestroyCurrentSceneEntities();

	SceneConfig CreateTestScene();


private:
	void AddMessageListner(const char* typeToListen, void* entToBindTo, std::function<void(mauvemessage::BaseMessage*)> functionToBind);
	std::unique_ptr<GraphicsManager> graphicsManager;
	SceneConfig currentScene;
};

#endif