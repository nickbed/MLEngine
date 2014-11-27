#include "SceneManager.h"

SceneManager::SceneManager(std::unique_ptr<GraphicsManager> graph)
{
	NULLPTRCHECK(graph, "Null graphicsmanager ptr passed to scene manager");
	graphicsManager = std::move(graph);
}

bool SceneManager::LoadScene(SceneConfig scene)
{
	currentScene = scene;

	//Init scene after we load in
	return InitCurrentScene();
}

SceneConfig SceneManager::LoadSceneFromFile(const char* fileName)
{
	//do json stuff here
	mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Load scene from file not implemented!");
	SceneConfig fail;
	return fail;
}

bool SceneManager::InitSceneManager()
{
	//Nothing yet
	return true;
}

bool SceneManager::InitCurrentScene()
{
	//Is anything our current scene? No entities = no scene
	if(currentScene.entities.size() == 0)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Called init scene on an empty scene");
		return false;
	}

	//Iterate through vector and init all entities
	for(std::vector<IEntity*>::iterator it = currentScene.entities.begin(); it != currentScene.entities.end(); ++it)
	{
		(*it)->Init();
	}

	return true;
}

bool SceneManager::UpdateCurrentSceneEntities(float dt)
{
	//iterate through vector and update all entities and their components
	//Iterate through vector and init all entities
	bool result = true;
	for(std::vector<IEntity*>::iterator it = currentScene.entities.begin(); it != currentScene.entities.end(); ++it)
	{
		result &= (*it)->Update(dt);
	}
	DrawCurrentSceneEntities(dt);
	return result;
}

bool SceneManager::DrawCurrentSceneEntities(float dt)
{
	//send entities to graphics manager to have them drawn
	//also send camera and light info
	graphicsManager->SetCurrentCamera(currentScene.currentSceneCamera);
	graphicsManager->SetCurrentSceneLight(currentScene.currentSceneLight);
	graphicsManager->SetCurrentShader(currentScene.currentSceneShader);
	graphicsManager->DrawAndUpdateWindow(currentScene.entities,dt);
	return true;
}

bool SceneManager::DestroyCurrentSceneEntities()
{
	for(std::vector<IEntity*>::iterator it = currentScene.entities.begin(); it != currentScene.entities.end(); ++it)
	{
		(*it)->Destroy();
		delete *it;
	}
	currentScene.entities.clear();
	//Delete everything from the vector and associated components
	return true;
}

SceneConfig SceneManager::CreateTestScene()
{
	SceneConfig testScene;

	//Create our scene here
	ThreeDGraphics* graphics = new ThreeDGraphics("threeDGraphics");
	ThreeDGraphics* graphics2 = new ThreeDGraphics("threeDGraphics2");

	GLfloat floatvert[]  = 
	{
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	GLfloat floatNorm[]  = 
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};

	GLfloat floatcol[] =
	{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f
	};

	GLfloat floatvert2[]  = 
	{
		-10.0f, -5.0f, -10.0f,
		-10.0f, -5.0f, 10.0f,
		10.0f, -5.0f, -10.0f,
		10.0f, -5.0f, -10.0f,
		-10.0f, -5.0f, 10.0f,
		10.0f, -5.0f, 10.0f
	};

	GLfloat floatcol2[] =
	{
		0.9f, 0.0f, 0.0f,
		0.0f, 0.9f, 0.0f,
		0.0f, 0.0f, 0.9f,
		0.9f, 0.0f, 0.0f,
		0.0f, 0.9f, 0.0f,
		0.0f, 0.0f, 0.9f
	};

	GLfloat floatNorm2[]  = 
	{
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	std::vector<GLfloat> testVertices(floatvert, floatvert + sizeof(floatvert) / sizeof(GLfloat));
	std::vector<GLfloat> testVertices2(floatvert2, floatvert2 + sizeof(floatvert2) / sizeof(GLfloat));

	std::vector<GLfloat> testColours(floatcol, floatcol + sizeof(floatcol) / sizeof(GLfloat));
	std::vector<GLfloat> testColours2(floatcol2, floatcol2 + sizeof(floatcol2) / sizeof(GLfloat));

	std::vector<GLfloat> testNormals(floatNorm, floatNorm + sizeof(floatNorm) / sizeof(GLfloat));
	std::vector<GLfloat> testNormals2(floatNorm2, floatNorm2 + sizeof(floatNorm2) / sizeof(GLfloat));

	graphics->UploadVertices(testVertices);
	graphics->UploadColours(testColours);
	graphics->UploadNormals(testNormals);
	
	graphics2->UploadVertices(testVertices2);
	graphics2->UploadColours(testColours2);
	graphics2->UploadNormals(testNormals2);

	GeneralEntity* testEntity = new GeneralEntity();
	GeneralEntity* testEntity2 = new GeneralEntity();

	testEntity->Components->AddComponent("testGraphics",graphics);
	testEntity2->Components->AddComponent("testGraphics", graphics2);


	//Add a camera
	CameraEntity* currentCamera = new CameraEntity();
	glm::vec3 newCameraPos = glm::vec3(0.0, 5.0, 50.0);
	currentCamera->SetPosition(glm::vec3(0.0f, 3.0f, 3.0f));

	BasicKeyMovement* movement = new BasicKeyMovement("keyboardMovement", graphicsManager->GetCurrentWindow());
	movement->Init();

	MousePoller* mouseReader = new MousePoller("mouseMovement", graphicsManager->GetCurrentWindow());
	mouseReader->Init();

	testScene.currentSceneLight = new SceneLight();

	//Set keyboard listner events
	//mauvemessage::RecieverInfo rec;
	//rec.listenobjectptr = currentCamera;

	//using namespace std::placeholders;
	//rec.listnerFunction = std::bind(&CameraEntity::msg_SetMovePosition, currentCamera, _1);


	//rec.typeToListen = "keyboardMovement";
	//mauvemessage::MessageManager::AddMessageListner("keyboardMovement", rec);
	using namespace std::placeholders;
	AddMessageListner("keyboardMovement", currentCamera, std::bind(&CameraEntity::msg_SetMovePosition, currentCamera, _1));

	AddMessageListner("mouseMovement", currentCamera, std::bind(&CameraEntity::msg_SetLookPosition, currentCamera, _1));

	AddMessageListner("cameraPositionMove", testScene.currentSceneLight, std::bind(&SceneLight::msg_LightPositionHandler, testScene.currentSceneLight, _1));

	AddMessageListner("cameraPositionMove", testEntity2, std::bind(&TransformComponent::msg_MoveToPosition, testEntity2->Transform, _1));

//#define LISTNERFUNCTION(function, currobj) using namespace std::placeholders; std::bind(function, currobj, _1);

	//Set mouse listner events
	//mauvemessage::RecieverInfo rec2;
	//rec2.listenobjectptr = currentCamera;

	//using namespace std::placeholders;
	//rec2.listnerFunction = std::bind(&CameraEntity::msg_SetLookPosition, currentCamera, _1);

	//rec2.typeToListen = "mouseMovement";
	//mauvemessage::MessageManager::AddMessageListner("mouseMovement", rec2);

	currentCamera->Components->AddComponent("keyboardMovement", movement);
	currentCamera->Components->AddComponent("mouseMovement", mouseReader);

	//Put stuff into the scene
	testScene.currentSceneCamera = currentCamera;
	testScene.currentSceneLight->lightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);
	testScene.currentSceneLight->surfaceReflectivity = glm::vec3(1.0f, 1.0f, 1.0f);
	testScene.currentSceneLight->lightPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	testScene.currentSceneShader = mauveresource::ResourceManager::GetResource<Shader>("data\\shaders\\default");
	testScene.cameras.push_back(currentCamera);
	testScene.currentSceneCamera = currentCamera;
	testScene.entities.push_back(testEntity);
	testScene.entities.push_back(testEntity2);
	return testScene;
}

void SceneManager::AddMessageListner(const char* typeToListen, void* entToBindTo, std::function<void(mauvemessage::BaseMessage*)> functionToBind)
{
	//Set mouse listner events
	mauvemessage::RecieverInfo rec;
	rec.listenobjectptr = entToBindTo;

	using namespace std::placeholders;
	rec.listnerFunction = functionToBind;

	rec.typeToListen = typeToListen;
	mauvemessage::MessageManager::AddMessageListner(typeToListen, rec);
}