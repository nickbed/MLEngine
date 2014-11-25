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

	graphicsManager = std::unique_ptr<GraphicsManager>(new GraphicsManager);
	graphicsManager->Init(3,3);
	graphicsManager->CreateGraphicsWindow(currentConfig.resX, currentConfig.resY, "Test window");
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

	GLfloat floatvert2[] =
	{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f
	};

	GLfloat floatvert3[]  = 
	{
		-10.0f, -1.0f, -10.0f,
		-10.0f, -1.0f, 10.0f,
		10.0f, -1.0f, -10.0f,
		10.0f, -1.0f, -10.0f,
		-10.0f, -1.0f, 10.0f,
		10.0f, -1.0f, 10.0f
	};

	GLfloat floatvert4[] =
	{
		0.9f, 0.0f, 0.0f,
		0.0f, 0.9f, 0.0f,
		0.0f, 0.0f, 0.9f,
		0.9f, 0.0f, 0.0f,
		0.0f, 0.9f, 0.0f,
		0.0f, 0.0f, 0.9f
	};

	std::vector<GLfloat> testVertices(floatvert, floatvert + sizeof(floatvert) / sizeof(GLfloat));
	std::vector<GLfloat> testVertices2(floatvert2, floatvert2 + sizeof(floatvert2) / sizeof(GLfloat));
	std::vector<GLfloat> testVertices3(floatvert3, floatvert3 + sizeof(floatvert3) / sizeof(GLfloat));
	std::vector<GLfloat> testVertices4(floatvert4, floatvert4 + sizeof(floatvert4) / sizeof(GLfloat));

	graphics->UploadVertices(testVertices);
	graphics->UploadColours(testVertices2);
	
	graphics2->UploadVertices(testVertices3);
	graphics2->UploadColours(testVertices4);

	testEntity = new GeneralEntity();


	testEntity->Components->AddComponent("testGraphics", graphics2);
	testEntity->Components->AddComponent("testGraphics",graphics);

	//Add a camera
	CameraEntity* currentCamera = new CameraEntity();
	glm::vec3 newCameraPos = glm::vec3(0.0, 5.0, 50.0);
	currentCamera->SetPosition(glm::vec3(0.0f, 3.0f, 3.0f));

	BasicKeyMovement* movement = new BasicKeyMovement("keyboardMovement", graphicsManager->GetCurrentWindow());
	movement->Init();

	MousePoller* mouseReader = new MousePoller("mouseMovement", graphicsManager->GetCurrentWindow());
	mouseReader->Init();



	//Set keyboard listner events
	mauvemessage::RecieverInfo rec;
	rec.listenobjectptr = currentCamera;

	using namespace std::placeholders;
	rec.listnerFunction = std::bind(&CameraEntity::msg_SetMovePosition, currentCamera, _1);


	rec.typeToListen = "keyboardMovement";
	mauvemessage::MessageManager::AddMessageListner("keyboardMovement", rec);

	//Set mouse listner events
	mauvemessage::RecieverInfo rec2;
	rec.listenobjectptr = currentCamera;

	using namespace std::placeholders;
	rec2.listnerFunction = std::bind(&CameraEntity::msg_SetLookPosition, currentCamera, _1);

	rec2.typeToListen = "mouseMovement";
	mauvemessage::MessageManager::AddMessageListner("mouseMovement", rec2);

	graphicsManager->SetCurrentCamera(currentCamera);

	currentCamera->Components->AddComponent("keyboardMovement", movement);
	currentCamera->Components->AddComponent("mouseMovement", mouseReader);
	

	//testEntity->Transform->SetScale(glm::vec3(0.5f, 0.5f, 1.0f));
}

bool Engine::Update(float dt)
{
	std::vector<IEntity*> testEnts;
	testEnts.push_back(testEntity);
	graphicsManager->DrawAndUpdateWindow(testEnts, dt);
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

