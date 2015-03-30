#include "SceneManager.h"

std::unique_ptr<SceneConfig> SceneManager::currentScene;

SceneManager::SceneManager(std::unique_ptr<GraphicsManager> graph)
{
	NULLPTRCHECK(graph, "Null graphicsmanager ptr passed to scene manager");
	graphicsManager = std::move(graph);
	isLoading = false;
	showDebug = false;
	shouldLoadLevel = false;
	lastDt = 0.0f;
	currentPlayer = nullptr;
}

SceneManager::~SceneManager()
{
	graphicsManager.release();
}

bool SceneManager::LoadScene(std::unique_ptr<SceneConfig> scene)
{
	currentScene = std::move(scene);
	std::stringstream s;
	s << "Mauve Engine - " << currentScene->filename;

	//Init scene after we load in
	graphicsManager->SetWindowTitle(s.str().c_str());
	currentPlayer = currentScene->currentPlayer;
	isLoading = false;
	return InitCurrentScene();
}

std::unique_ptr<SceneConfig> SceneManager::LoadSceneFromFile(const char* filePath)
{
	CollisionSystem::ClearVolumes();
	shouldLoadLevel = false;
	bool menu;
	graphicsManager->SetWindowTitle("Mauve Engine - Loading....");
	showDebug = false;
	mauvemessage::MessageManager::ClearAllListners();
	int loadingTextX = 60;
	int loadingTextY = 100;
	int loadingTextSize = 50;
	IEntity* gotEntities = nullptr;
	if(currentScene != nullptr)
	{
		gotEntities = currentScene->activeEntities[0];
	}
	std::unique_ptr<SceneConfig> gotConfig = std::unique_ptr<SceneConfig>(new SceneConfig, std::default_delete<SceneConfig>());
	gotConfig->filename = filePath;
	JSONFile* gotJSON = mauvefileresource::ResourceManager::LoadResource<JSONFile>(filePath);
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
						entToCreate->id = entID.c_str();
					}
					if (entType == "robot")
					{
						entToCreate = new Robot();
						entToCreate->id = entID.c_str();
						using namespace std::placeholders;
						Robot* tempRobot = (Robot*)entToCreate;
						AddMessageListner("robotMovement", tempRobot, std::bind(&Robot::msg_SetMovePosition, tempRobot, std::placeholders::_1));
					}

					//Iterate over data inside the entity (components, transform, script)
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
										ImageTexture* gotTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>(gotTexturePath);
										if (gotTexture == nullptr)
										{
											//Use the no texture found texture
											gotTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>("data\\images\\default.png");
										}

										GPUTexture* gotGPUTexture = mauvegpuresource::GPUResourceManager::GetResource<GPUTexture>(gotTexturePath);
										if (gotGPUTexture == nullptr)
										{
											gotGPUTexture = new GPUTexture();
											gotGPUTexture->SetTexture(gotTexture);
											gotGPUTexture = mauvegpuresource::GPUResourceManager::LoadResource<GPUTexture>(gotGPUTexture, gotTexturePath);
											
										}
										gotComponent->SetTexture(*gotGPUTexture);
										OBJModel* gotModel = mauvefileresource::ResourceManager::GetResource<OBJModel>(gotOBJPath);
										if (gotModel == nullptr) success = false;
										else
										{
											//Put the obj data in the thing
											GPUModel* gotGPUModel = mauvegpuresource::GPUResourceManager::GetResource<GPUModel>(gotOBJPath);
											if (gotGPUModel == nullptr)
											{
												//Create and load in new gpu model
												gotGPUModel = new GPUModel();
												gotGPUModel->SetAllData(gotModel->GetVertices(), gotModel->GetVertexCount(), gotModel->GetNormals(), gotModel->GetNormalCount(), gotModel->GetUVs(), gotModel->GetUVCount(), gotModel->GetIndexCount());
												gotGPUModel = mauvegpuresource::GPUResourceManager::LoadResource(gotGPUModel, gotOBJPath);
											}
											gotComponent->SetModel(*gotGPUModel);
											success &= entToCreate->Components->AddComponent(componentContents["type"].asString(), gotComponent);
										}
									}
								}
								//if (componentContents["type"] == "basicbone")
								//{
								//	graphicsManager->RenderText("Loading bone",loadingTextX,loadingTextY,loadingTextSize,gotEntities);
								//	BasicBone* gotComponent = new BasicBone(componentContents["id"].asString());
								//	
								//	//Load in the obj file specified
								//	std::string gotOBJPath = componentContents["OBJModel"].asString();

								//	//Load in the texture location
								//	std::string gotTexturePath = componentContents["TextureFile"].asString();

								//	if (gotOBJPath.size() == 0) success = false;
								//	else
								//	{
								//		//Load texture
								//		ImageTexture* gotTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>(gotTexturePath);
								//		if (gotTexture == nullptr)
								//		{
								//			//Use the no texture found texture
								//			gotTexture = mauvefileresource::ResourceManager::GetResource<ImageTexture>("data\\images\\default.png");
								//		}

								//		OBJModel* gotModel = mauvefileresource::ResourceManager::GetResource<OBJModel>(gotOBJPath);
								//		if (gotModel == nullptr) success = false;
								//		else
								//		{
								//			//Put the obj data in the thing
								//			gotComponent->UploadUVs(gotModel->GetUVs());
								//			gotComponent->UploadVertices(gotModel->GetVertices());
								//			gotComponent->UploadNormals(gotModel->GetNormals());
								//			gotComponent->UploadIndices(gotModel->GetIndicies());
								//			gotComponent->UploadTexture(gotTexture->GetBitmap());
								//			gotComponent->BoneTransform.SetPosition(glm::vec3(-50.0f,0.0f,0.0f));
								//			success &= entToCreate->Components->AddComponent(componentContents["type"].asString(), gotComponent);

								//		}
								//	}
								//}
								if (componentContents["type"] == "boundingbox")
								{
									AddBoundingBox(componentContents,entToCreate);
								}
								if (componentContents["type"] == "boundingboxo")
								{
									AddBoundingBoxO(componentContents,entToCreate);
								}
								if (componentContents["type"] == "boundingcapsule")
								{
									AddBoundingCapsule(componentContents,entToCreate);
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

						//*************************SCRIPT HANDLING**************************************
						//*************************SCRIPT HANDLING**************************************
						//*************************SCRIPT HANDLING**************************************
						else if (key2.asString() == "script")
						{
							entToCreate->Script->Load(value2["path"].asString(), value2["identifier"].asString());
							AddMessageListner("msg_collision", entToCreate, std::bind(&ScriptComponent::msg_Collision, entToCreate->Script, std::placeholders::_1));
							if(value2["identifier"].asString()=="Coin")
							{
								gotConfig->coin++;
							}
						}
					}
					//Put entity into our map
					std::string entIDtoadd = entID;
					entToCreate->id = entIDtoadd.c_str();
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

					if (value.isMember("script"))
					{
						Json::Value value2 = value["script"];
						camToCreate->Script->Load(value2["path"].asString(), value2["identifier"].asString());
					}
				}

			}

			//Create and setup lights
			const Json::Value& jsonLights = sceneData["lights"];
			graphicsManager->RenderText("Loading lights", loadingTextX, loadingTextY, 60, gotEntities);
			GenerateLightsFromJson(jsonLights, *gotConfig->sceneLights);


			//Read scene setup current values last
			std::string sceneShader = sceneData["sceneshader"].asCString();
			std::string sceneCamera = sceneData["currentcamera"].asCString();
			bool messageHandlers = sceneData["messagehandlers"].asBool();
			menu = sceneData["menu"].asBool();
			
			//Iterate through all active entities and put them in the vector
			Json::Value activeEntities = sceneData["activeentities"];
			for (unsigned int i = 0; i < activeEntities.size(); i++)
			{
				std::string gotEntid = activeEntities[i].asString();
				IEntity* gotEntPtr = gotConfig->sceneEntities->find(gotEntid)->second;
				gotConfig->activeEntities[gotConfig->numActiveEntities++] = gotEntPtr;
			}

			Json::Value activeLightsJSON = sceneData["activelights"];
			int gotSize = activeLightsJSON.size();
			if (gotSize> 128) mauveassert::Assert::HandleException("Too many lights in JSON file");
			else gotConfig->numActiveLights = gotSize;
			for (unsigned int i = 0; i < gotSize; i++)
			{
				std::string gotLightid = activeLightsJSON[i].asString();
				SceneLight* gotLightPtr = gotConfig->sceneLights->find(gotLightid)->second;
				gotConfig->activeLights[i] = *gotLightPtr;
			}


			//Put the active shader in the current shader
			gotConfig->currentSceneShader = mauvefileresource::ResourceManager::GetResource<Shader>(sceneShader);

			//Put the active camera in the scene
			gotConfig->currentSceneCamera = gotConfig->sceneCameras->find(sceneCamera)->second;

			
			if (menu || messageHandlers)
			{
				using namespace std::placeholders;
				//Hack - needs to be data driven
				BasicKeyMovement* movement = new BasicKeyMovement("keyboardMovement", graphicsManager->GetCurrentWindow());
				movement->Init();

				//MousePoller* mouseReader = new MousePoller("mouseMovement", graphicsManager->GetCurrentWindow());
				//mouseReader->Init();

				CameraEntity* dummyCamera = new CameraEntity();

				gotConfig->sceneCameras->insert(std::pair<std::string, CameraEntity*>("dummy", dummyCamera));

				//TODO - abstract into the input manager
				dummyCamera->Components->AddComponent("keyboardMovement", movement);

				//dummyCamera->Components->AddComponent("mouseMovement", mouseReader);

				if (menu)
				{
					AddMessageListner("loadGame", this, std::bind(&SceneManager::msg_LoadGame, this, std::placeholders::_1));
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
		gotConfig->currentPlayer = (IEntity*)gotConfig->sceneEntities->find("robot1")->second;
		AddMessageListner("mouseMovement", (Robot*)gotConfig->sceneEntities->find("robot1")->second, std::bind(&Robot::msg_SetHeadPosition, (Robot*)gotConfig->sceneEntities->find("robot1")->second, std::placeholders::_1));
	}

	AddMessageListner("robotPositionMove", (CameraEntity*)gotConfig->sceneCameras->find("camera1")->second, std::bind(&CameraEntity::msg_SetFollowPosition, (CameraEntity*)gotConfig->sceneCameras->find("camera1")->second, std::placeholders::_1));

	AddMessageListner("robotPositionMove", this, std::bind(&SceneManager::msg_RobotPosition, this, std::placeholders::_1));

	AddMessageListner("setCamera", this, std::bind(&SceneManager::msg_SetCamera, this, std::placeholders::_1));
	
	
	
	//bad
	CameraEntity* camToSetup = gotConfig->sceneCameras->find("camera1")->second;

	AddMessageListner("cameraMovement", camToSetup, std::bind(&CameraEntity::msg_SetMovePosition, camToSetup, std::placeholders::_1));

	AddMessageListner("mouseMovement", camToSetup, std::bind(&CameraEntity::msg_SetLookPosition, camToSetup, std::placeholders::_1));

	AddMessageListner("cameraMovement", gotConfig->currentSceneCamera, std::bind(&CameraEntity::msg_SetMovePosition, gotConfig->currentSceneCamera, std::placeholders::_1));

	AddMessageListner("mouseMovement", gotConfig->currentSceneCamera, std::bind(&CameraEntity::msg_SetLookPosition, gotConfig->currentSceneCamera, std::placeholders::_1));

	//For reloading the scene
	AddMessageListner("reloadScene", this, std::bind(&SceneManager::msg_ReloadScene, this, std::placeholders::_1));

	AddMessageListner("showDebug", this, std::bind(&SceneManager::msg_ShowDebug, this, std::placeholders::_1));

	
	delete gotJSON;
	return std::move(gotConfig);
}


void SceneManager::AddBoundingBox(Json::Value contents, IEntity* entToCreate)
{
	bool gotStatic = contents["static"].asBool();
    glm::vec3 gotCenter = glm::vec3(contents["centerX"].asFloat(), contents["centerY"].asFloat(), contents["centerZ"].asFloat());
    float gotExtent = contents["extent"].asFloat();
    float gotRadius = contents["radius"].asFloat();
    BoundingCapsule* gotComponent = new BoundingCapsule("boundingcapsule",gotCenter,gotRadius,gotExtent,gotStatic,entToCreate->id);
    //gotComponent->SetTransform(entToCreate->Transform);
    entToCreate->Components->AddComponent(contents["type"].asString(), gotComponent);
    if(gotStatic==true)
    {
            CollisionSystem::AddStaticVolume(gotComponent);
    }
    else
    {
            CollisionSystem::AddDynamicVolume(gotComponent);
    }
}

void SceneManager::AddBoundingBoxO(Json::Value contents, IEntity* entToCreate)
{
	bool gotStatic = contents["static"].asBool();
	float gotDensity;
	float gotFriction;
	float gotPush;
	if(contents.isMember("density"))
	{
		gotDensity = contents["density"].asFloat();
	}
	else
	{
		gotDensity = 1.f;
	}
	if(contents.isMember("friction"))
	{
		gotFriction = contents["friction"].asFloat();
	}
	else
	{
		gotFriction = 1.f;
	}
	if(contents.isMember("push"))
	{
		gotPush = contents["push"].asFloat();
	}
	else
	{
		gotPush = 0.f;
	}
	glm::vec3 gotCenter = glm::vec3(contents["centerX"].asFloat(), contents["centerY"].asFloat(), contents["centerZ"].asFloat());
	glm::vec3 gotExtent = glm::vec3(contents["extentX"].asFloat(), contents["extentY"].asFloat(), contents["extentZ"].asFloat());
	BoundingBoxO* gotComponent = new BoundingBoxO("boundingbox",gotCenter,gotExtent,gotStatic,entToCreate->id);
	gotComponent->Rigid_density = gotDensity;
	gotComponent->Rigid_friction = gotFriction;

	gotComponent->Rigid_mass = (gotExtent.x*gotExtent.y*gotExtent.z)*gotDensity;
	gotComponent->Rigid_inverse = 1.f / (gotComponent->Rigid_mass);



	//gotComponent->SetTransform(entToCreate->Transform);
	entToCreate->Components->AddComponent(contents["type"].asString(), gotComponent);
	if(gotStatic==true)
	{
		CollisionSystem::AddStaticVolume(gotComponent);
	}
	else
	{
		CollisionSystem::AddDynamicVolume(gotComponent);
	}
}

void SceneManager::AddBoundingCapsule(Json::Value contents, IEntity* entToCreate)
{
	float gotFriction;
	float gotPush;
	float gotDensity;
	bool gotStatic = contents["static"].asBool();
	glm::vec3 gotCenter = glm::vec3(contents["centerX"].asFloat(), contents["centerY"].asFloat(), contents["centerZ"].asFloat());
	float gotExtent = contents["extent"].asFloat();
	float gotRadius = contents["radius"].asFloat();
	if(contents.isMember("density"))
	{
		gotDensity = contents["density"].asFloat();
	}
	else
	{
		gotDensity = 1.f;
	}
	if(contents.isMember("friction"))
	{
		gotFriction = contents["friction"].asFloat();
	}
	else
	{
		gotFriction = 1.f;
	}
	if(contents.isMember("push"))
	{
		gotPush = contents["push"].asFloat();
	}
	else
	{
		gotPush = 0.f;
	}
	BoundingCapsule* gotComponent = new BoundingCapsule("boundingcapsule",gotCenter,gotRadius,gotExtent,gotStatic,entToCreate->id);
	gotComponent->Rigid_density=gotDensity;
	gotComponent->Rigid_friction=gotFriction;
	gotComponent->Rigid_push=gotPush;
	//gotComponent->SetTransform(entToCreate->Transform);
	entToCreate->Components->AddComponent(contents["type"].asString(), gotComponent);
	if(gotStatic==true)
	{
		CollisionSystem::AddStaticVolume(gotComponent);
	}
	else
	{
		CollisionSystem::AddDynamicVolume(gotComponent);
	}
}


bool SceneManager::InitSceneManager()
{
	//Nothing yet
	return true;
}

bool SceneManager::InitCurrentScene()
{
	graphicsManager->currentParticleManager->ClearAllParticleSystems();
	//Is anything our current scene? No entities = no scene
	NULLPTRCHECK(currentScene, "Nullptr on current scene when trying to init");
	if(currentScene == nullptr)
	{
		return false;
	}
	if(currentScene->numActiveEntities== 0)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Called init scene on an empty scene");
		return false;
	}

	graphicsManager->SetActiveSceneLights(currentScene->numActiveLights, currentScene->activeLights);
	graphicsManager->SetCurrentCamera(currentScene->currentSceneCamera);
	graphicsManager->SetCurrentShader(currentScene->currentSceneShader);
	graphicsManager->currentParticles = graphicsManager->currentParticleManager->AddNewParticleSystem(glm::vec3(-1.0), glm::vec3(0.0), "data\\images\\fire.png");
	graphicsManager->currentParticles2 = graphicsManager->currentParticleManager->AddNewParticleSystem(glm::vec3(-1.0), glm::vec3(0.0), "data\\images\\fire.png");

	ParticleSystem* part = graphicsManager->currentParticleManager->AddNewParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -8.9f, 0.0f), "data\\images\\waterdrop.png");
	part->SetParticleSize(8.0f);
	//part->SetLifetime(100.0f);
	//part->SetAcceleration(glm::vec3(0.0f, -5.0f, 0.0f), false);
	part->SetPosition(glm::vec3(0.0f, 45.0f, 0.0f), true);
	part->SetUseRandomPositionRange(true, -100.0f, -100.0f, 100.0f, 100.0f);
	//Iterate through vector and init all entities
	/*for (std::vector<IEntity*>::iterator it = currentScene->activeEntities.begin(); it != currentScene->activeEntities.end(); ++it)
	{
		(*it)->Init();
	}*/

	return true;
}

bool SceneManager::UpdateCurrentSceneEntities(float dt)
{
	if (currentPlayer != nullptr)
	{
		float offsetangle = glm::radians(currentPlayer->getTransform()->GetRotation().y);
		glm::vec4 pointOffset, pointOffset2;
		pointOffset.y = 0.0f;
		pointOffset.z = 1.0f;
		pointOffset.x = 1.0f;
		pointOffset.w = 1.0f;

		pointOffset2.y = 0.0f;
		pointOffset2.z = 1.0f;
		pointOffset2.x = 1.0f;
		pointOffset2.w = 1.0f;

		glm::mat4 rotTranslate;
		rotTranslate *= glm::rotate(glm::degrees(offsetangle), glm::vec3(0.0, 1.0, 0.0));
		rotTranslate *= glm::translate(glm::vec3(-0.9, -0.18, -0.8));
		//rotTranslate = glm::translate(glm::vec3(0.1, 0.0, 0.0));
		pointOffset = rotTranslate * pointOffset;

		rotTranslate *= glm::translate(glm::vec3(-0.2, 0.0, 0.0));
		pointOffset2 = rotTranslate * pointOffset2;

		glm::vec3 point1 = currentPlayer->getTransform()->GetPosition()-0.01f;
		point1 -= currentPlayer->getTransform()->GetPosition();
		static float angle = 0;
		angle+=0.1;
		glm::vec3 n;
		n.x = (point1.x *  glm::sin(angle)) * -1.8f;
		n.z = (point1.z * glm::cos(angle)) * -1.8f;
		n.y = 0.55f;
		point1 = n+currentPlayer->getTransform()->GetPosition() ;
		graphicsManager->currentParticles->SetPosition(point1 + glm::vec3(pointOffset.x, pointOffset.y, pointOffset.z), true);
	    graphicsManager->currentParticles2->SetPosition(point1 + glm::vec3(pointOffset2.x, pointOffset2.y, pointOffset2.z), true);

		rotTranslate = glm::mat4(1.0);
		rotTranslate *= glm::scale(glm::mat4(1.0f), glm::vec3(10.0, 1.0, 10.0));
		rotTranslate *= glm::rotate(-25.0f, glm::vec3(0.0, 1.0, 0.0));
		pointOffset = rotTranslate * pointOffset;
		graphicsManager->currentParticles->SetAcceleration(glm::vec3(pointOffset.x, -0.2f, pointOffset.z), true);
		graphicsManager->currentParticles2->SetAcceleration(glm::vec3(pointOffset.x, -0.2f, pointOffset.z), true);

	}
	lastDt = dt;
	//iterate through vector and update all entities and their components
	//Iterate through vector and init all entities
	bool result = true;

	/*int entCount = currentScene->activeEntities.size();
	auto it = currentScene->activeEntities.begin();
	while (it != currentScene->activeEntities.end())
	{
		result &= (*it)->Update(dt);
		if (entCount != currentScene->activeEntities.size())
		{
			it = currentScene->activeEntities.end();
		}
		else {
			++it;
		}
	}*/

	for (int i = 0; i < currentScene->numActiveEntities; ++i)
	{
		result &= currentScene->activeEntities[i]->Update(dt);
	}

	//for(std::map<std::string, CameraEntity*>::iterator it = currentScene->sceneCameras->begin(); it != currentScene->sceneCameras->end(); ++it)
	//	{
	//		CameraEntity* gotCamera = (*it).second;
	//		gotCamera->Update(dt);
	//}
	currentScene->currentSceneCamera->Listning = true;
	currentScene->currentSceneCamera->Update(dt);
	graphicsManager->PollWindow();
	if(currentScene->sceneCameras->find("dummy") != currentScene->sceneCameras->end())
	{
		currentScene->sceneCameras->find("dummy")->second->Update(dt);
	}
	if(result == false) return false;
	
	if(isLoading) ReloadScene();
	return result;
}

bool SceneManager::DrawCurrentSceneEntities(float dt)
{
	
	//send entities to graphics manager to have them drawn
	//also send camera and light info
	
	bool result = graphicsManager->DrawAndUpdateWindow(currentScene->activeEntities, currentScene->numActiveEntities,lastDt, true);
	if (showDebug)
	{
		//update msec
		std::stringstream s;
		float update = lastDt*1000.0f;
		s << "Update msec: " << update;
		graphicsManager->RenderText(s.str().c_str(), 5, 720, 30);

		//Scene file
		s.str(std::string());
		s << "Scene: " << currentScene->filename;
		graphicsManager->RenderText(s.str().c_str(), 5, 650, 30);

		//Num active ents
		s.str(std::string());
		s << "Active Ents: " << currentScene->numActiveEntities;
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
		s << "Light x: " << currentScene->activeLights[0].lightPosition.x;
		graphicsManager->RenderText(s.str().c_str(), 5, 400, 30);

		s.str(std::string());
		s << "Light y: " << currentScene->activeLights[0].lightPosition.y;
		graphicsManager->RenderText(s.str().c_str(), 5, 350, 30);

		s.str(std::string());
		s << "Light z: " << currentScene->activeLights[0].lightPosition.z;
		graphicsManager->RenderText(s.str().c_str(), 5, 300, 30);

		s.str(std::string());
		s << "Ent list";
		graphicsManager->RenderText(s.str().c_str(), 750, 720, 30);

		int startpos = 710;
		//std::map<std::string, IEntity*>::iterator startit = currentScene->sceneEntities->begin();
		for (auto it = currentScene->sceneEntities->begin(); it != currentScene->sceneEntities->end(); ++it)
		{
			s.str(std::string());
			s << it->first;
			graphicsManager->RenderText(s.str().c_str(), 750, startpos, 15);
			startpos -= 10;
			graphicsManager->DrawDebug(it->second);
		}
	}
	else
	{
		if(currentScene->coin > 0)
		{
			std::string coin = "Coin: " + std::to_string(currentScene->coin);
			graphicsManager->RenderText(coin.c_str(), 5, 720, 30);
		}
	}
	return result;
}

bool SceneManager::DestroyCurrentSceneEntities()
{
	if(currentScene == nullptr) return true;
	for (int i = 0; i < currentScene->numActiveEntities; ++i)
	{
		currentScene->activeEntities[i]->Destroy();
		delete currentScene->activeEntities[i];
		currentScene->activeEntities[i] = nullptr;
	}
	return true;
}

SceneConfig SceneManager::CreateTestScene()
{
	SceneConfig testScene;
	return testScene;
}

void SceneManager::AddMessageListner(const char* typeToListen, void* entToBindTo, std::function<void(mauvemessage::BaseMessage*)> functionToBind)
{
	//Set mouse listener events
	mauvemessage::RecieverInfo rec;
	rec.listenobjectptr = entToBindTo;

	using namespace std::placeholders;
	rec.listnerFunction = functionToBind;

	rec.typeToListen = typeToListen;
	mauvemessage::MessageManager::AddMessageListner(typeToListen, rec);
}

void SceneManager::GenerateLightsFromJson(const Json::Value& jsonLights, std::map<std::string, SceneLight*>& lights)
{
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
		lights.insert(std::pair<std::string, SceneLight*>(lightIDtoadd, lightToCreate));
	}
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
		graphicsManager->SetCurrentCamera(currentScene->currentSceneCamera);
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


IEntity*  SceneManager::AddEntity(std::string id, bool isActive)
{
	IEntity* entToAdd = new IEntity;
	currentScene->sceneEntities->insert(std::pair<std::string, IEntity*>(id, entToAdd));
	if (isActive)
	{
		//currentScene->activeEntities.push_back((currentScene->sceneEntities->find(id)->second));
		currentScene->activeEntities[currentScene->numActiveEntities++] = currentScene->sceneEntities->find(id)->second;
	}

	return currentScene->sceneEntities->find(id)->second;
}

void SceneManager::RemoveCoin()
{
	--currentScene->coin;
}
void SceneManager::DestroyEntity(std::string id)
{
	IEntity* entToRemove = currentScene->sceneEntities->find(id)->second;
	entToRemove->Destroy();
	for (int i = 0; i < currentScene->numActiveEntities; ++i)
	{
		if (currentScene->activeEntities[i] == entToRemove)
		{
			currentScene->numActiveEntities -= 1;
			for (int j = 0; j < currentScene->numActiveEntities - i; ++j)
			{
				currentScene->activeEntities[i + j] = currentScene->activeEntities[i + j + 1];
			}
			return;
		}
	}
	//std::vector<IEntity*>::iterator position = std::find(currentScene->activeEntities.begin(), currentScene->activeEntities.end(), entToRemove);
	//if (position != currentScene->activeEntities.end()) // == vector.end() means the element was not found
	//	currentScene->activeEntities.erase(position);
}
IEntity* SceneManager::FindEntity(std::string id)
{
	if (currentScene->sceneEntities->find(id) == currentScene->sceneEntities->end())
	{
		return nullptr;
	}
	IEntity* entToFind = currentScene->sceneEntities->find(id)->second;
	return entToFind;
}
void SceneManager::msg_RobotPosition(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	
}
