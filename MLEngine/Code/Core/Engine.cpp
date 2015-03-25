#include "Engine.h"


Engine::Engine()
{
}

Engine::~Engine()
{
	currentState.release();
	delete sceneManager;
	delete inputManager;
	scriptManager.Close();
	CollisionSystem::Destroy();
}

void Engine::Init(EngineConfig conf)
{
	currentConfig = conf;
	//Init message manager
	listners = new std::unordered_multimap<const char*, mauvemessage::RecieverInfo>();
	mauvemessage::MessageManager::LoadMap(listners);

	//Init script system
	if (!scriptManager.Init())
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Unable to initialize Lua scripting environment.");
	}

	//Init graphics here
	std::unique_ptr<GraphicsManager> graphicsMan = std::unique_ptr<GraphicsManager>(new GraphicsManager);
	graphicsMan->Init(3,3);
	graphicsMan->CreateGraphicsWindow(currentConfig.resX, currentConfig.resY, "Mauve Engine");

	inputManager = new InputSystem();
	inputManager->Init(graphicsMan->GetCurrentWindow());

	physicsManager = new PhysicsSystem();
	physicsManager->Init();

	//Init scene manager here
	sceneManager = new SceneManager(std::move(graphicsMan));
	sceneManager->InitSceneManager();

	std::unique_ptr<SceneConfig> newScene = sceneManager->LoadSceneFromFile("data\\scenes\\menu.scn");
	sceneManager->LoadScene(std::move(newScene));
}

bool Engine::Update(float dt)
{
	bool result = true;
	result &= inputManager->Update(dt);
	result &= sceneManager->UpdateCurrentSceneEntities(dt);
	result &= physicsManager->Update(dt);
	CollisionSystem::CheckCollisions();
	if(sceneManager->ShouldLoadLevel())
	{
			std::unique_ptr<SceneConfig> newScene = sceneManager->LoadSceneFromFile("data\\scenes\\loading.scn");
			sceneManager->LoadScene(std::move(newScene));
		    sceneManager->UpdateCurrentSceneEntities(dt);

		    std::unique_ptr<SceneConfig> newScene2 = sceneManager->LoadSceneFromFile("data\\scenes\\demolevel.scn");
			sceneManager->LoadScene(std::move(newScene2));
			loading = false;
			timer = 0;
			physicsManager->Init();
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
	return false;
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
		JSONFile* configJson = mauvefileresource::ResourceManager::GetResource<JSONFile>(configFile);
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
		//delete configJson;
		return jsonConfig;
}
