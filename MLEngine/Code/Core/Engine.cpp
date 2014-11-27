#include "Engine.h"


Engine::Engine(const EngineConfig conf) : currentConfig(conf)
{
}

Engine::~Engine()
{
	currentState.release();
}

void Engine::Init()
{
	//Init message manager
	listners = new std::unordered_multimap<const char*, mauvemessage::RecieverInfo>();
	mauvemessage::MessageManager messageHandler = mauvemessage::MessageManager(listners);

	//Init graphics here
	std::unique_ptr<GraphicsManager> graphicsMan = std::unique_ptr<GraphicsManager>(new GraphicsManager);
	graphicsMan->Init(3,3);
	graphicsMan->CreateGraphicsWindow(currentConfig.resX, currentConfig.resY, "Test window");

	//Init scene manager here
	sceneManager = new SceneManager(std::move(graphicsMan));
	sceneManager->InitSceneManager();
	SceneConfig newScene = sceneManager->CreateTestScene();
	sceneManager->LoadScene(newScene);
	
	

	//testEntity->Transform->SetScale(glm::vec3(0.5f, 0.5f, 1.0f));
}

bool Engine::Update(float dt)
{
	sceneManager->UpdateCurrentSceneEntities(dt);
	return true;
}

bool Engine::LoadNextState()
{
	//Cache our next state so it doesn't get destroyed
	std::unique_ptr<IState> nextState = std::move(this->currentState->GetNextState());
	
	//Check to see if we have a next state, if we don't then handle it elsewhere in the engine.
	if(nextState == nullptr)
	{
		//handle this in our update loop
		return false;
	}

	//Clean up our old current state
	this->currentState->StateDestroy();
	this->currentState.release();

	//Plop our new state into 
	this->currentState = std::move(nextState);

	//Init our new state
	this->currentState->StateInit();

	//Success
	return true;
}

