#ifndef ENGINE_H
#define ENGINE_H
#include "../Assert/Assert.h"
#include "../States/TestState.h"
#include "..\Graphics\GraphicsManager.h"
#include "..\Entities\GeneralEntity.h"
#include "../States/TestState.h"
#include "../Messages/MessageManager.h"
#include "..\Scenes\SceneManager.h"


#include "..\Components\StaticMeshNoIndices.h"
#include "..\Components\BasicKeyMovement.h"
#include "..\Components\MousePoller.h"

#include "json\json.h"

struct EngineConfig
{
	int openglMajVersion;
	int openglMinVersion;
	int resX;
	int resY;
	bool antiAliasing;
	bool asserts;
	const char* defaultShader;
};

class Engine
{
public:
	//Constructor/destructor
	Engine();
	virtual ~Engine();

	//Init the engine with config
	void Init(const EngineConfig conf);

	//Update the engine
	bool Update(float dt);
	void Draw();

	//Reading config files
	EngineConfig ReadConfigFile(const char* configFile);

private:
	//State stuff
	//States will be a collection of systems that all have their own Init/Update/Finish
	std::unordered_multimap<const char*, mauvemessage::RecieverInfo>* listners;
	std::unique_ptr<IState> currentState;
	std::unique_ptr<GraphicsManager> graphicsManager;
	GeneralEntity* testEntity;

	EngineConfig currentConfig;

	SceneManager* sceneManager;

	//Load the next state into current state from the current state 
	//(ie - put the next state into the current state and release the pointer for the old state)
	bool LoadNextState();
	int loops;
};

#endif
