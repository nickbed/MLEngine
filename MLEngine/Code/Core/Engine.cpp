#include "Engine.h"
#include "../States/TestState.h"
#include "../Messages/MessageManager.h"


#include "..\Components\threeDGraphics.h"

Engine::Engine(const EngineConfig conf) : currentConfig(conf)
{
}

Engine::~Engine()
{
	currentState.release();
}

void Engine::Init()
{
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
	

	//testEntity->Transform->SetScale(glm::vec3(0.5f, 0.5f, 1.0f));
}

bool Engine::Update()
{
	std::vector<IEntity*> testEnts;
	testEnts.push_back(testEntity);
	graphicsManager->DrawAndUpdateWindow(testEnts, 0.0f);
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

