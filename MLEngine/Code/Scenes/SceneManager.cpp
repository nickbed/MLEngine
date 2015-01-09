#include "SceneManager.h"

SceneManager::SceneManager(std::unique_ptr<GraphicsManager> graph)
{
	NULLPTRCHECK(graph, "Null graphicsmanager ptr passed to scene manager");
	graphicsManager = std::move(graph);
	isLoading = false;
	showDebug = false;
	shouldLoadLevel = false;
}

bool SceneManager::LoadScene(std::unique_ptr<SceneConfig> scene)
{
	currentScene = std::move(scene);
	std::stringstream s;
	s << "Mauve Engine - " << currentScene->filename;

	//Init scene after we load in
	graphicsManager->SetWindowTitle(s.str().c_str());
	return InitCurrentScene();
	isLoading = false;
}

std::unique_ptr<SceneConfig> SceneManager::LoadSceneFromFile(const char* filePath)
{
	shouldLoadLevel = false;
	bool menu;
	graphicsManager->SetWindowTitle("Mauve Engine - Loading....");
	showDebug = false;
	mauvemessage::MessageManager::ClearAllListners();
	int loadingTextX = 60;
	int loadingTextY = 100;
	int loadingTextSize = 50;
	std::vector<IEntity*> gotEntities;
	if(currentScene != nullptr)
	{
		gotEntities = currentScene->activeEntities;
	}
	std::unique_ptr<SceneConfig> gotConfig = std::unique_ptr<SceneConfig>(new SceneConfig, std::default_delete<SceneConfig>());
	gotConfig->filename = filePath;
	JSONFile* gotJSON = mauveresource::ResourceManager::LoadResource<JSONFile>(filePath);
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
			else if (success)
			{
				//Iterate over all entities
				for (Json::Value::const_iterator it = jsonEntities.begin(); it != jsonEntities.end(); ++it)
				{
					IEntity* entToCreate = nullptr;
					Json::Value value = (*it);
					std::string entType = value["type"].asString();
					std::string entID = it.key().asString();
					char result[100];
					sprintf_s(result, "Loading entityID: \n%s", entID.c_str());
					graphicsManager->RenderText(result,loadingTextX,loadingTextY,loadingTextSize,gotEntities);
					if (entType == "generalentity")
					{
						entToCreate = new GeneralEntity();
					}
					if (entType == "robot")
					{
						entToCreate = new Robot();
						using namespace std::placeholders;
						Robot* tempRobot = (Robot*)entToCreate;
						AddMessageListner("robotMovement", tempRobot, std::bind(&Robot::msg_SetMovePosition, tempRobot, _1));
					}

					//Iterate over data inside the entity (components, transform)
					for (Json::Value::iterator it2 = value.begin(); it2 != value.end(); ++it2)
					{
						Json::Value key2 = it2.key();
						Json::Value value2 = (*it2);
#ifdef JSON_PARSE_OUTPUT
						std::cout << "Key: " << key2.toStyledString();
						std::cout << "Value: " << value2.toStyledString();
#endif

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

								char result[100];
								sprintf_s(result, "Loading component: \n%s", componentID.asCString());
								graphicsManager->RenderText(result,loadingTextX,loadingTextY,loadingTextSize,gotEntities);
								
								if (componentContents["type"] == "staticmesh")
								{
									graphicsManager->RenderText("Loading static mesh",loadingTextX,loadingTextY,loadingTextSize,gotEntities);
									StaticMesh* gotComponent = new StaticMesh(componentContents["id"].asString());
									
									//Load in the obj file specified
									std::string gotOBJPath = componentContents["OBJModel"].asString();
									char result[100];
									sprintf_s(result, "Loading OBJ: %s", gotOBJPath.c_str());
									graphicsManager->RenderText(result,loadingTextX,loadingTextY,loadingTextSize,gotEntities);

									//Load in the texture location
									std::string gotTexturePath = componentContents["TextureFile"].asString();

									if (gotOBJPath.size() == 0) success = false;
									else
									{
										//Load texture
										ImageTexture* gotTexture = mauveresource::ResourceManager::GetResource<ImageTexture>(gotTexturePath);
										if (gotTexture == nullptr)
										{
											//Use the no texture found texture
											gotTexture = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\default.png");
										}

										OBJModel* gotModel = mauveresource::ResourceManager::GetResource<OBJModel>(gotOBJPath);
										if (gotModel == nullptr) success = false;
										else
										{
											//Put the obj data in the thing
											gotComponent->UploadUVs(gotModel->GetUVs());
											gotComponent->UploadVertices(gotModel->GetVertices());
										    gotComponent->UploadNormals(gotModel->GetNormals());
											gotComponent->UploadIndices(gotModel->GetIndicies());

											gotComponent->UploadTexture(gotTexture->GetBitmap());
											success &= entToCreate->Components->AddComponent(componentContents["type"].asString(), gotComponent);
										}
									}
								}
								if (componentContents["type"] == "basicbone")
								{
									graphicsManager->RenderText("Loading bone",loadingTextX,loadingTextY,loadingTextSize,gotEntities);
									BasicBone* gotComponent = new BasicBone(componentContents["id"].asString());
									
									//Load in the obj file specified
									std::string gotOBJPath = componentContents["OBJModel"].asString();

									//Load in the texture location
									std::string gotTexturePath = componentContents["TextureFile"].asString();

									if (gotOBJPath.size() == 0) success = false;
									else
									{
										//Load texture
										ImageTexture* gotTexture = mauveresource::ResourceManager::GetResource<ImageTexture>(gotTexturePath);
										if (gotTexture == nullptr)
										{
											//Use the no texture found texture
											gotTexture = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\default.png");
										}

										OBJModel* gotModel = mauveresource::ResourceManager::GetResource<OBJModel>(gotOBJPath);
										if (gotModel == nullptr) success = false;
										else
										{
											//Put the obj data in the thing
											gotComponent->UploadUVs(gotModel->GetUVs());
											gotComponent->UploadVertices(gotModel->GetVertices());
										    gotComponent->UploadNormals(gotModel->GetNormals());
											gotComponent->UploadIndices(gotModel->GetIndicies());
											gotComponent->UploadTexture(gotTexture->GetBitmap());
											gotComponent->BoneTransform.SetPosition(glm::vec3(-50.0f,0.0f,0.0f));
											success &= entToCreate->Components->AddComponent(componentContents["type"].asString(), gotComponent);

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
							entToCreate->Transform->SetRotation(gotRotation);
							entToCreate->Transform->SetScale(gotScale);
						}
					}
					//Put entity into our map
					std::string entIDtoadd = entID;
					gotConfig->sceneEntities->insert(std::pair<std::string, IEntity*>(entIDtoadd, entToCreate));
				}
			}
			//Create and setup cameras
			const Json::Value& jsonCameras = sceneData["cameras"];
			if (jsonCameras.isNull())
			{
				success = false;
			}
			else if (success)
			{
				graphicsManager->RenderText("Loading cameras",loadingTextX,loadingTextY,60,gotEntities);
				//Iterate over all cameras
				for (Json::Value::const_iterator it = jsonCameras.begin(); it != jsonCameras.end(); ++it)
				{
					CameraEntity* camToCreate = new CameraEntity();
					Json::Value value = (*it);
					std::string cameraID = it.key().asString();

					Json::Value camSetup = value["camerasetup"];

					glm::vec3 camPosition = glm::vec3(camSetup["posX"].asFloat(), camSetup["posY"].asFloat(), camSetup["posZ"].asFloat());
					glm::vec3 lookPosition = glm::vec3(camSetup["lookX"].asFloat(), camSetup["lookY"].asFloat(), camSetup["lookZ"].asFloat());
					float fov = camSetup["fov"].asFloat();
					float pitch = camSetup["pitch"].asFloat();
					float yaw = camSetup["yaw"].asFloat();

					//Load values into camera
					camToCreate->SetPosition(camPosition);
					camToCreate->SetLookPosition(lookPosition);
					camToCreate->SetFov(fov);
					camToCreate->SetPitch(pitch);
					camToCreate->SetYaw(yaw);

					//Put the camera in the map
					std::string cameraIDtoadd = cameraID;
					gotConfig->sceneCameras->insert(std::pair<std::string, CameraEntity*>(cameraIDtoadd, camToCreate));

				}

			}

			//Create and setup lights
			const Json::Value& jsonLights = sceneData["lights"];
			if (jsonLights.isNull())
			{
				success = false;
			}
			else if (success)
			{
				graphicsManager->RenderText("Loading lights",loadingTextX,loadingTextY,60,gotEntities);
				//Iterate over all lights
				for (Json::Value::const_iterator it = jsonLights.begin(); it != jsonLights.end(); ++it)
				{
					SceneLight* lightToCreate = new SceneLight();
					Json::Value value = (*it);
					std::string lightID = it.key().asString();

					Json::Value lightSetup = value["lightparams"];

					glm::vec3 lightPosition = glm::vec3(lightSetup["posX"].asFloat(), lightSetup["posY"].asFloat(), lightSetup["posZ"].asFloat());
					glm::vec3 lightIntensity = glm::vec3(lightSetup["intensityR"].asFloat(), lightSetup["intensityG"].asFloat(), lightSetup["intensityB"].asFloat());
					glm::vec3 lightReflectivity = glm::vec3(lightSetup["intensityR"].asFloat(), lightSetup["intensityG"].asFloat(), lightSetup["intensityB"].asFloat());

					//Load values into light
					lightToCreate->lightPosition = lightPosition;
					lightToCreate->lightIntensity = lightIntensity;
					lightToCreate->surfaceReflectivity = lightReflectivity;

					//Put the light in the map
					std::string lightIDtoadd = lightID;
					gotConfig->sceneLights->insert(std::pair<std::string, SceneLight*>(lightIDtoadd, lightToCreate));
				}
			}
			if(!success)
			{
				return nullptr;
			}

			//TODO - Guard all this

			//Read scene setup current values last
			std::string sceneShader = sceneData["sceneshader"].asCString();
			std::string sceneCamera = sceneData["currentcamera"].asCString();
			std::string sceneLight = sceneData["currentlight"].asCString();
			bool messageHandlers = sceneData["messagehandlers"].asBool();
			menu = sceneData["menu"].asBool();
			
			//Iterate through all active entities and put them in the vector
			Json::Value activeEntities = sceneData["activeentities"];
			for (unsigned int i = 0; i < activeEntities.size(); i++)
			{
				std::string gotEntid = activeEntities[i].asString();
				IEntity* gotEntPtr = gotConfig->sceneEntities->find(gotEntid)->second;
				gotConfig->activeEntities.push_back((gotConfig->sceneEntities->find(gotEntid)->second));
			}

			//Put the active shader in the current shader
			gotConfig->currentSceneShader = mauveresource::ResourceManager::GetResource<Shader>(sceneShader);

			//Put the active camera in the scene
			gotConfig->currentSceneCamera = gotConfig->sceneCameras->find(sceneCamera)->second;

			//Put the active light in the scene
			gotConfig->currentSceneLight = gotConfig->sceneLights->find(sceneLight)->second;
			
			
			if (menu || messageHandlers)
			{
				using namespace std::placeholders;
				//Hack - needs to be data driven
				BasicKeyMovement* movement = new BasicKeyMovement("keyboardMovement", graphicsManager->GetCurrentWindow());
				movement->Init();

				MousePoller* mouseReader = new MousePoller("mouseMovement", graphicsManager->GetCurrentWindow());
				mouseReader->Init();

				CameraEntity* dummyCamera = new CameraEntity();

				gotConfig->sceneCameras->insert(std::pair<std::string, CameraEntity*>("dummy", dummyCamera));

				//TODO - abstract into the input manager
				dummyCamera->Components->AddComponent("keyboardMovement", movement);

				dummyCamera->Components->AddComponent("mouseMovement", mouseReader);

				if (menu)
				{
					AddMessageListner("loadGame", this, std::bind(&SceneManager::msg_LoadGame, this, _1));
				}
			}

			if (!messageHandlers) return std::move(gotConfig);
		}
	}
	if(!success)
	{
		return std::move(nullptr);
	}

	
	using namespace std::placeholders;
	
	//TODO - fixthis

	//if(currentScene != nullptr)
	//{
	//	int check = 0;
	//	for(std::map<std::string, CameraEntity*>::iterator itz = currentScene->sceneCameras->begin(); itz != currentScene->sceneCameras->end(); ++itz)
	//	{
	//		++check;
	//		//if(check > 2) break;
	//		CameraEntity* gotCamera = (*itz).second;
	//		AddMessageListner("cameraMovement", gotCamera, std::bind(&CameraEntity::msg_SetMovePosition, gotCamera, _1));

	//		AddMessageListner("mouseMovement", gotCamera, std::bind(&CameraEntity::msg_SetLookPosition, gotCamera, _1));


	//	}
	//}



	if(gotConfig->sceneEntities->find("robot1") != gotConfig->sceneEntities->end())
	{
		AddMessageListner("mouseMovement",(Robot*)gotConfig->sceneEntities->find("robot1")->second, std::bind(&Robot::msg_SetHeadPosition, (Robot*)gotConfig->sceneEntities->find("robot1")->second, _1));
	}

	AddMessageListner("robotPositionMove", (CameraEntity*)gotConfig->sceneCameras->find("camera1")->second, std::bind(&CameraEntity::msg_SetFollowPosition, (CameraEntity*)gotConfig->sceneCameras->find("camera1")->second, _1));

	AddMessageListner("setCamera", this, std::bind(&SceneManager::msg_SetCamera, this, _1));
	
	
	
	//bad

	CameraEntity* camToSetup = gotConfig->sceneCameras->find("camera1")->second;

	AddMessageListner("cameraMovement", camToSetup, std::bind(&CameraEntity::msg_SetMovePosition, camToSetup, _1));

	AddMessageListner("mouseMovement", camToSetup, std::bind(&CameraEntity::msg_SetLookPosition, camToSetup, _1));

	AddMessageListner("cameraMovement", gotConfig->currentSceneCamera, std::bind(&CameraEntity::msg_SetMovePosition, gotConfig->currentSceneCamera, _1));

	AddMessageListner("mouseMovement", gotConfig->currentSceneCamera, std::bind(&CameraEntity::msg_SetLookPosition, gotConfig->currentSceneCamera, _1));

	//For realoding the scene
	AddMessageListner("reloadScene", this, std::bind(&SceneManager::msg_ReloadScene, this, _1));

	AddMessageListner("showDebug", this, std::bind(&SceneManager::msg_ShowDebug, this, _1));

	

	return std::move(gotConfig);
}

bool SceneManager::InitSceneManager()
{
	//Nothing yet
	return true;
}

bool SceneManager::InitCurrentScene()
{
	//Is anything our current scene? No entities = no scene
	NULLPTRCHECK(currentScene, "Nullptr on current scene when trying to init");
	if(currentScene == nullptr)
	{
		return false;
	}
	if(currentScene->activeEntities.size() == 0)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Called init scene on an empty scene");
		return false;
	}

	//Iterate through vector and init all entities
	/*for (std::vector<IEntity*>::iterator it = currentScene->activeEntities.begin(); it != currentScene->activeEntities.end(); ++it)
	{
		(*it)->Init();
	}*/

	return true;
}

bool SceneManager::UpdateCurrentSceneEntities(float dt)
{
	//iterate through vector and update all entities and their components
	//Iterate through vector and init all entities
	bool result = true;
	for (std::vector<IEntity*>::iterator it = currentScene->activeEntities.begin(); it != currentScene->activeEntities.end(); ++it)
	{
		result &= (*it)->Update(dt);
	}

	//for(std::map<std::string, CameraEntity*>::iterator it = currentScene->sceneCameras->begin(); it != currentScene->sceneCameras->end(); ++it)
	//	{
	//		CameraEntity* gotCamera = (*it).second;
	//		gotCamera->Update(dt);
	//}
	currentScene->currentSceneCamera->Listning = true;
	currentScene->currentSceneCamera->Update(dt);
	if(currentScene->sceneCameras->find("dummy") != currentScene->sceneCameras->end())
	{
		currentScene->sceneCameras->find("dummy")->second->Update(dt);
	}
	result &= DrawCurrentSceneEntities(dt);
	if(showDebug)
	{
		//update msec
		std::stringstream s;
		float update = dt*1000.0f;
		s << "Update msec: " << update;
		graphicsManager->RenderText(s.str().c_str(),5,720,30);

		//Scene file
		s.str(std::string());
		s << "Scene: " << currentScene->filename;
		graphicsManager->RenderText(s.str().c_str(), 5, 650, 30);

		//Num active ents
		s.str(std::string());
		s << "Active Ents: " << currentScene->activeEntities.size();
		graphicsManager->RenderText(s.str().c_str(), 5, 600, 30);
		
		//Camera pos
		s.str(std::string());
		s << "Cam x: " << currentScene->currentSceneCamera->GetCameraPosition().x;
		graphicsManager->RenderText(s.str().c_str(), 5, 550, 30);

		s.str(std::string());
		s << "Cam y: " << currentScene->currentSceneCamera->GetCameraPosition().y;
		graphicsManager->RenderText(s.str().c_str(), 5, 500, 30);

		s.str(std::string());
		s << "Cam z: " << currentScene->currentSceneCamera->GetCameraPosition().z;
		graphicsManager->RenderText(s.str().c_str(), 5, 450, 30);

		//Light pos
		s.str(std::string());
		s << "Light x: " << currentScene->currentSceneLight->lightPosition.x;
		graphicsManager->RenderText(s.str().c_str(), 5, 400, 30);

		s.str(std::string());
		s << "Light y: " << currentScene->currentSceneLight->lightPosition.y;
		graphicsManager->RenderText(s.str().c_str(), 5, 350, 30);

		s.str(std::string());
		s << "Light z: " << currentScene->currentSceneLight->lightPosition.z;
		graphicsManager->RenderText(s.str().c_str(), 5, 300, 30);

		s.str(std::string());
		s << "Ent list";
		graphicsManager->RenderText(s.str().c_str(), 750, 720, 30);

		int startpos = 710;
		std::map<std::string, IEntity*>::iterator startit = currentScene->sceneEntities->begin();
		for (auto it = currentScene->sceneEntities->begin(); it != currentScene->sceneEntities->end(); ++it)
		{
			s.str(std::string());
			s << it->first;
			graphicsManager->RenderText(s.str().c_str(), 750, startpos, 15);
			startpos -= 10;
		}
	}
	if(isLoading) ReloadScene();
	return result;
}

bool SceneManager::DrawCurrentSceneEntities(float dt)
{
	
	//send entities to graphics manager to have them drawn
	//also send camera and light info
	graphicsManager->SetCurrentCamera(currentScene->currentSceneCamera);
	graphicsManager->SetCurrentSceneLight(currentScene->currentSceneLight);
	graphicsManager->SetCurrentShader(currentScene->currentSceneShader);
	return graphicsManager->DrawAndUpdateWindow(currentScene->activeEntities,dt, true);
}

bool SceneManager::DestroyCurrentSceneEntities()
{
	if(currentScene == nullptr) return true;
	for (std::vector<IEntity*>::iterator it = currentScene->activeEntities.begin(); it != currentScene->activeEntities.end(); ++it)
	{
		(*it)->Destroy();
		delete *it;
	}
	currentScene->activeEntities.clear();
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

	currentCamera->Components->AddComponent("keyboardMovement", movement);
	currentCamera->Components->AddComponent("mouseMovement", mouseReader);

	//Put stuff into the scene
	testScene.currentSceneCamera = currentCamera;
	testScene.currentSceneLight->lightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);
	testScene.currentSceneLight->surfaceReflectivity = glm::vec3(1.0f, 1.0f, 1.0f);
	testScene.currentSceneLight->lightPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	testScene.currentSceneShader = mauveresource::ResourceManager::GetResource<Shader>("data\\shaders\\default");
	
	//testScene.sceneCameras->push_back(currentCamera);
	//testScene.currentSceneCamera = currentCamera;
	//testScene.entities.push_back(testEntity);
	//testScene.entities.push_back(testEntity2);
	//testScene.entities.push_back(testEntity3);
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

void SceneManager::msg_SetCamera(mauvemessage::BaseMessage* msg)
{
	//Get camera number from the message
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	std::string camToSearch;
	int camNum = (int)std::ceil(messagePos.x);
	if(camNum == 1)
	{
		camToSearch = "camera0";
	}
	else if(camNum == 2)
	{
		camToSearch = "camera1";
	}
	else if(camNum == 3)
	{
		camToSearch = "camera2";
	}
	else if(camNum == 4)
	{
		camToSearch = "camera3";
	}
	CameraEntity* cam = currentScene->sceneCameras->at(camToSearch);
	std::map<std::string, CameraEntity*>::iterator gotCamera = currentScene->sceneCameras->find(camToSearch);
	if(gotCamera != currentScene->sceneCameras->end())
	{
		currentScene->currentSceneCamera->Listning = false;
		currentScene->currentSceneCamera = cam;
		currentScene->currentSceneCamera->Listning = true;
	}
}

void SceneManager::msg_ReloadScene(mauvemessage::BaseMessage* msg)
{
	isLoading = true;
}

void SceneManager::msg_ShowDebug(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	int debug = (int)std::ceil(messagePos.x);
	if(debug == 1)
	{
		showDebug = true;
	}
	else
	{
		showDebug = false;
	}
}

void SceneManager::msg_LoadGame(mauvemessage::BaseMessage* msg)
{
	shouldLoadLevel = true;
}

void SceneManager::ReloadScene()
{
	std::unique_ptr<SceneConfig> scn = LoadSceneFromFile(currentScene->filename.c_str());
	LoadScene(std::move(scn));
	isLoading = false;
}
bool SceneManager::ShouldLoadLevel()
{
	return shouldLoadLevel;
}

