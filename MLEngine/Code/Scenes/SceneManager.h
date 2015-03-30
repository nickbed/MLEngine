#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "..\Assert\Assert.h"
#include "..\Resources\GPUResourceManager.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Interfaces\IEntity.h"
#include "..\resources\Shader.h"
#include "..\Resources\OBJModel.h"
#include "..\Resources\JSONFile.h"
#include "..\entities\Camera.h"
#include "..\entities\GeneralEntity.h"
#include "..\entities\Robot.h"
#include "..\Components\BasicKeyMovement.h"
#include "..\Components\BoundingBox.h"
#include "..\Components\BoundingBoxO.h"
#include "..\Components\BoundingSphere.h"
#include "..\Components\BoundingCapsule.h"
#include "..\Components\MousePoller.h"
#include "..\Components\StaticMeshNoIndices.h"
#include "..\Components\StaticMesh.h"
#include "..\Messages\BaseMessage.h"
#include "..\Systems\CollisionSystem.h"
#include "SceneTypes.h"
#include <vector>
#include <map>
#include <sstream>


class SceneManager
{
public:

	SceneManager(std::unique_ptr<GraphicsManager> graph);
	~SceneManager();

	//Load scene into engine
	bool LoadScene(std::unique_ptr<SceneConfig> scene);

	//Generate scene from file
	std::unique_ptr<SceneConfig> LoadSceneFromFile(const char* fileName);

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

	void msg_SetCamera(mauvemessage::BaseMessage* msg);

	void msg_ReloadScene(mauvemessage::BaseMessage* msg);

	void msg_ShowDebug(mauvemessage::BaseMessage* msg);

	void msg_LoadGame(mauvemessage::BaseMessage* msg);

	void msg_RobotPosition(mauvemessage::BaseMessage* msg);

	void ReloadScene();

	bool ShouldLoadLevel();

	//Lua
	static void RemoveCoin();
	static IEntity* AddEntity(std::string id, bool isActive);
	static void DestroyEntity(std::string id);
	static IEntity* FindEntity(std::string id);
	static void LoadLevel(std::string id);

private:
	void AddMessageListner(const char* typeToListen, void* entToBindTo, std::function<void(mauvemessage::BaseMessage*)> functionToBind);
	void GenerateLightsFromJson(const Json::Value& jsonLights, std::map<std::string, SceneLight*>& lights);

	void AddBoundingBox(Json::Value contents, IEntity* entToCreate);
	void AddBoundingBoxO(Json::Value contents, IEntity* entToCreate);
	void AddBoundingCapsule(Json::Value contents, IEntity* entToCreate);

	std::unique_ptr<GraphicsManager> graphicsManager;
	static std::unique_ptr<SceneConfig> currentScene;
	IEntity* currentPlayer;
	bool isLoading;
	bool showDebug;
	float lastDt;

	static bool shouldLoadLevel;
	static std::string nextLevel;
};

#endif