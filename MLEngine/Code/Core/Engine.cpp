#include "Engine.h"


Engine::Engine()
{
}

Engine::~Engine()
{
	currentState.release();
	sceneManager->DestroyCurrentSceneEntities();
}

void Engine::Init(EngineConfig conf)
{
	currentConfig = conf;
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
	std::unique_ptr<SceneConfig> newScene = sceneManager->LoadSceneFromFile("data\\scenes\\loading.scn");
	//SceneConfig newScene = sceneManager->CreateTestScene();
	sceneManager->LoadScene(std::move(newScene));
	loading = true;
}

bool Engine::Update(float dt)
{
	bool result = true;
	result &= sceneManager->UpdateCurrentSceneEntities(dt);
	timer += dt;
	if(loading)
	{
		    sceneManager->UpdateCurrentSceneEntities(dt);
		    std::unique_ptr<SceneConfig> newScene = sceneManager->LoadSceneFromFile("data\\scenes\\demolevel.scn");
			sceneManager->LoadScene(std::move(newScene));
			loading = false;
			timer = 0;
	}
	//else if(timer > 5.0f)
	//{
	//	std::unique_ptr<SceneConfig> newScene = sceneManager->LoadSceneFromFile("data\\scenes\\demolevel.scn");
	//	sceneManager->LoadScene(std::move(newScene));
	//	timer = 0.0f;
	//}
	return result;
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

EngineConfig Engine::ReadConfigFile(const char* configFile)
{
		//Fallback if we can't read the config
		EngineConfig defaultConfig;
		defaultConfig.openglMajVersion = 3;
		defaultConfig.openglMinVersion = 3;
		defaultConfig.resX = 1024;
		defaultConfig.resY = 768;
		defaultConfig.antiAliasing = false;
		defaultConfig.asserts = false;
		defaultConfig.defaultShader = "\\data\\shaders\\default";

		bool success = true;
		EngineConfig jsonConfig;
		JSONFile* configJson = mauveresource::ResourceManager::GetResource<JSONFile>(configFile);
		if (configJson == nullptr)
		{
			success = false;
		}
		else
		{
			const Json::Value& configValues = configJson->GetJsonKeyValue("engineconfig");
			if (configValues == 0) success = false;
			if (!configValues.isNull() && success)
			{
				try
				{
					jsonConfig.openglMajVersion = configValues["openglmaj"].asInt();
					jsonConfig.openglMinVersion = configValues["openglmin"].asInt();
					jsonConfig.resX = configValues["resX"].asInt();
					jsonConfig.resY = configValues["resY"].asInt();
					jsonConfig.antiAliasing = configValues["antialiasing"].asBool();
					jsonConfig.asserts = configValues["asserts"].asBool();
					jsonConfig.defaultShader = configValues["defaultshader"].asCString();
				}
				catch (std::exception)
				{
					success = false;
				}
			}
		}
		if (!success)
		{
			mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Error reading engine config json file, using default.");
			return defaultConfig;
		}
		return jsonConfig;
}

