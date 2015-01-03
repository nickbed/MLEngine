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

SceneConfig SceneManager::LoadSceneFromFile(const char* filePath)
{
	SceneConfig gotConfig;
	JSONFile* gotJSON = mauveresource::ResourceManager::GetResource<JSONFile>(filePath);
	bool success = true;
	if (gotJSON == nullptr)
	{
		success = false;
	}
	else
	{
		const Json::Value& sceneData = gotJSON->GetJsonKeyValue("scene");
		if (sceneData == 0) success = false;
		if (!sceneData.isNull() && success)
		{
			//Create and setup entities
			const Json::Value& jsonEntities = sceneData["entities"];
			if (jsonEntities.isNull())
			{
				success = false;
			}
			else
			{
				//Iterate over all entities
				for (Json::Value::const_iterator it = jsonEntities.begin(); it != jsonEntities.end(); ++it)
				{
					IEntity* entToCreate = nullptr;
					Json::Value value = (*it);
					std::string entType = value["type"].asString();
					if (entType == "generalentity")
					{
						entToCreate = new GeneralEntity();
					}

					//Iterate over data inside the entity (components, transform)
					for (Json::Value::iterator it2 = value.begin(); it2 != value.end(); ++it2)
					{
						Json::Value key2 = it2.key();
						Json::Value value2 = (*it2);
						std::cout << "Key: " << key2.toStyledString();
						std::cout << "Value: " << value2.toStyledString();

						//*************************COMPONENT HANDLING**************************************
						//*************************COMPONENT HANDLING**************************************
						//*************************COMPONENT HANDLING**************************************
						if (key2.asString() == "components")
						{
							//Iterate over component data
							for (Json::Value::iterator it3 = value2.begin(); it3 != value2.end(); ++it3)
							{
								Json::Value componentID = it3.key();
								Json::Value componentContents = (*it3);

								
								if (componentContents["type"] == "staticmesh")
								{
									 StaticMesh* gotComponent = new StaticMesh("staticmesh");
									
									//Load in the obj file specified
									std::string gotOBJPath = componentContents["OBJModel"].asCString();
									if (gotOBJPath.size() == 0) success = false;
									else
									{
										OBJModel* gotModel = mauveresource::ResourceManager::GetResource<OBJModel>(gotOBJPath);
										if (gotModel == nullptr) success = false;
										else
										{
											//Put the obj data in the thing
											gotComponent->UploadVertices(gotModel->GetVertices());
											gotComponent->UploadNormals(gotModel->GetNormals());
											gotComponent->UploadIndices(gotModel->GetIndicies());
											success &= entToCreate->Components->AddComponent(componentContents["id"].asCString(), gotComponent);
										}
									}
								}
							}
						}

						//*************************TRANSFORM HANDLING**************************************
						//*************************TRANSFORM HANDLING**************************************
						//*************************TRANSFORM HANDLING**************************************
						else if (key2.asString() == "transform")
						{
							glm::vec3 gotPosition = glm::vec3(value2["posX"].asFloat(), value2["posY"].asFloat(), value2["posZ"].asFloat());
							glm::vec3 gotRotation = glm::vec3(value2["rotateX"].asFloat(), value2["rotateY"].asFloat(), value2["rotateZ"].asFloat());
							glm::vec3 gotScale = glm::vec3(value2["scaleX"].asFloat(), value2["scaleY"].asFloat(), value2["scaleZ"].asFloat());

							entToCreate->Transform->SetPosition(gotPosition);
							entToCreate->Transform->SetPosition(gotRotation);
							entToCreate->Transform->SetPosition(gotScale);
						}
					}
				}
			}
			//Create and setup cameras
			//Create and setup lights

			//Read scene setup current values last
		}
	}
	return gotConfig;
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

	currentScene.currentSceneCamera->Update(dt);
	result &= DrawCurrentSceneEntities(dt);
	return result;
}

bool SceneManager::DrawCurrentSceneEntities(float dt)
{
	//send entities to graphics manager to have them drawn
	//also send camera and light info
	graphicsManager->SetCurrentCamera(currentScene.currentSceneCamera);
	graphicsManager->SetCurrentSceneLight(currentScene.currentSceneLight);
	graphicsManager->SetCurrentShader(currentScene.currentSceneShader);
	return graphicsManager->DrawAndUpdateWindow(currentScene.entities,dt);
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
	StaticMeshNoIndices* graphics = new StaticMeshNoIndices("StaticMeshNoIndices");
	StaticMeshNoIndices* graphics2 = new StaticMeshNoIndices("StaticMeshNoIndices2");
	StaticMesh* graphics3 = new StaticMesh("StaticMesh");
	OBJModel* testLeon = mauveresource::ResourceManager::GetResource<OBJModel>("data\\models\\leon.obj");

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
		-50.0f, -5.0f, -50.0f,
		-50.0f, -5.0f, 50.0f,
		50.0f, -5.0f, -50.0f,
		50.0f, -5.0f, -50.0f,
		-50.0f, -5.0f, 50.0f,
		50.0f, -5.0f, 50.0f
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

	graphics3->UploadVertices(testLeon->GetVertices());
	graphics3->UploadNormals(testLeon->GetNormals());
	graphics3->UploadIndices(testLeon->GetIndicies());

	GeneralEntity* testEntity = new GeneralEntity();
	GeneralEntity* testEntity2 = new GeneralEntity();
	GeneralEntity* testEntity3 = new GeneralEntity();

	testEntity->Components->AddComponent("testGraphics",graphics);
	testEntity2->Components->AddComponent("testGraphics", graphics2);
	testEntity3->Components->AddComponent("staticmesh", graphics3);
	testEntity3->Transform->SetScale(glm::vec3(1.0, 1.0, 1.0));


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
	testScene.entities.push_back(testEntity3);
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