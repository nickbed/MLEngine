#include "Scene\Scene.h"
#include <sstream>
std::string Scene::activeCamera = "default";
std::unordered_map<std::string, Camera*> Scene::cameras;
Scene::Scene()
{
	rManager = ResourceManager::getInstance();
	std::pair<std::string, Camera*> cameraPair;
	cameraPair.first = "default";
	cameraPair.second = new Camera();
	cameras.insert(cameraPair);
	cameras.at("default")->init();
	cameras.find("default")->second->setName("default");
	for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		*it = NULL;
	}
}
Scene::~Scene()
{
	for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	for(auto it = cameras.begin(); it != cameras.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}

}
bool Scene::LoadScene(const std::string& filename)
{
	Json::Reader reader;
	Json::Value root;
	std::fstream input;



	//std::size_t found;
	input.open(filename);
	if(!input.is_open())
	{
		std::cout << "file not found use .scn file extension" << std::endl;
		return false;
	}
	bool parsingSuccessful = reader.parse( input, root );
	if ( !parsingSuccessful )
	{
		// report to the user the failure and their locations in the document.
		std::cout  << "Failed to parse configuration\n" << reader.getFormattedErrorMessages();
		return 0;
	}
	///Storing the scene data from JSON
	sceneData.name = root["scene"]["name"].asString();
	ShaderLoader sLoader;
	if(sLoader.LoadShader(root["scene"]["sceneshader"].asString()+".vert", root["scene"]["sceneshader"].asString()+".Frag"))
	{
		sceneData.sceneShader = root["scene"]["sceneshader"].asString();
		if (rManager->getShaders_const().find(sceneData.sceneShader) == rManager->getShaders_const().end() ) 
		{
			ShaderLoader m_sloader;
			m_sloader.LoadShader(sceneData.sceneShader+".vert",sceneData.sceneShader+".Frag");
			Shader* shader = new Shader();
			shader->fragShader = m_sloader.getFrag();
			shader->vertShader = m_sloader.getVert();
			shader->programhandle = m_sloader.getProgramHandle();
			std::pair<std::string, Shader*> shaderPair;
			shaderPair.first = m_sloader.getName();
			shaderPair.second = shader;
			rManager->addToShader(shaderPair);
		}
	}
	else 
	{
		sceneData.sceneShader = "data\\shaders\\basic3.vert";
	}

	activeCamera = sceneData.currentCamera = root["scene"]["currentcamera"].asString();
	sceneData.currentLight = root["scene"]["currentlight"].asString();
	sceneData.messageHandlers =	root["scene"]["messagehandlers"].asBool();
	sceneData.menu = root["scene"]["menu"].asBool();
	///Creating a new Entity.

	for(Json::ValueIterator entsIter = root["scene"]["entities"].begin(); entsIter != root["scene"]["entities"].end(); ++entsIter)
	{
		int j =0;
		for(unsigned int i = 0; i < root["scene"]["activeentities"].size();++i)
		{
			Json::Value entityKey = entsIter.key();
			Json::Value entity = (*entsIter);

			if(entityKey.asString() == root["scene"]["activeentities"][j].asString())
			{

				GameObject *g = new GameObject(entityKey.asString());
				for(Json::ValueIterator it2 = entity.begin(); it2 != entity.end(); ++it2)
				{
					Json::Value key = it2.key();
					Json::Value value = (*it2);
					//cout << key.asString() << endl;
					if(key.asString() == "type" )
					{
						g->setEntityType(value.asString());
					}
					if(key.asString() == "transform" )
					{
						float posX, posY, posZ;
						float rotX, rotY, rotZ;
						float scaleX, scaleY, scaleZ;
						for(Json::ValueIterator transIter = value.begin(); transIter != value.end(); ++transIter)
						{
							Json::Value transKey = transIter.key();
							Json::Value transVal = (*transIter);
							std::string objString, textureString;

							if(transKey.asString() == "posX")
							{
								posX = transVal.asFloat();
							}
							if(transKey.asString() == "posY")
							{
								posY = transVal.asFloat();
							}
							if(transKey.asString() == "posZ")
							{
								posZ = transVal.asFloat();
							}
							if(transKey.asString() == "scaleX")
							{
								scaleX = transVal.asFloat();
							}
							if(transKey.asString() == "scaleY")
							{
								scaleY = transVal.asFloat();
							}
							if(transKey.asString() == "scaleZ")
							{
								scaleZ = transVal.asFloat();
							}
							if(transKey.asString() == "rotateX")
							{
								rotX = transVal.asFloat();
							}
							if(transKey.asString() == "rotateY")
							{
								rotY = transVal.asFloat();
							}
							if(transKey.asString() == "rotateZ")
							{
								rotZ = transVal.asFloat();
							}

						}
						g->getTransformComp()->Translate(posX,posY,posZ);
						g->getTransformComp()->Scale(scaleX, scaleY, scaleZ);
						g->getTransformComp()->Rotate(rotX, rotY, rotZ);
					}
					//ModelLoader modelLoader;
					if(key.asString() == "components" )
					{
						for(Json::ValueIterator compIter = value.begin(); compIter != value.end(); ++compIter)
						{
							Json::Value compKey = compIter.key();
							Json::Value compVal = (*compIter);
							std::string objString, textureString;

							for(Json::ValueIterator compValIter = compVal.begin(); compValIter != compVal.end(); ++compValIter)
							{
								Json::Value compVal2 = (*compValIter);
								Json::Value compValKey = compValIter.key();
								if(compValKey.asString() == "id")
								{
									g->addToComponentID(compVal2.asString());

								}
								if(compValKey.asString() == "type")
								{
									g->addToComponentTYPE(compVal2.asString());

								}
								if(compValKey.asString() == "OBJModel")
								{

									objString = compVal2.asString();
									if (rManager->getModel_const().find(objString) == rManager->getModel_const().end() ) 
									{
										ModelLoader* m_Loader = new ModelLoader();
										Model* m = new Model();
										m_Loader->loadFromfile(objString);
										m->verts = m_Loader->getVerts();
										m->normals = m_Loader->getNormals();
										m->textureCoords = m_Loader->getTextureCoords();
										rManager->addToModel(std::pair<std::string, Model*>(objString,m));
									}

									/*if(!rManager->getModel().at(objString))
									{
									Model* m = new Model();
									std::pair<std::string,Model*> modelPair;
									modelPair.first = objString;
									}*/


									g->addToComponentModelFiles(compVal2.asString());
								}
								if(compValKey.asString() == "TextureFile")
								{
									textureString = compVal2.asString();
									g->addToComponentTextureFiles(compVal2.asString());
									if (rManager->getTextures_const().find(textureString) == rManager->getTextures_const().end() ) 
									{
										TextureLoader* t_Loader = new TextureLoader();
										t_Loader->LoadTexture(textureString);
										t_Loader->FlipImage();
										rManager->addToTexture(std::pair<std::string, Texture*>(textureString,t_Loader->getTexture()));
									}
								}
							}
							g->getRenderComp()->init(rManager->getModel().at(objString), rManager->getTexture().at(textureString));
							//g->getTransformComp()->Rotate(
						}
					}
					//cout << value.asString() << endl;
				}

				gameObjects.push_back(g);
			}
			++j;
		}
	}
	for(Json::ValueIterator lightIter = root["scene"]["lights"].begin(); lightIter != root["scene"]["lights"].end(); ++lightIter)
	{
		Json::Value lightKey = lightIter.key();
		Json::Value lightVal = (*lightIter);
		Light light;
		light.name = lightKey.asString();
		for(Json::ValueIterator lightParamIt = lightVal["lightparams"].begin(); lightParamIt != lightVal["lightparams"].end();++lightParamIt)
		{
			Json::Value lightParamKey = lightParamIt.key();
			Json::Value lightParamVal = (*lightParamIt);
			if(lightParamKey.asString() == "posX")
			{
				light.position.x = lightParamVal.asFloat();
			}
			if(lightParamKey.asString() == "posY")
			{
				light.position.y = lightParamVal.asFloat();

			}
			if(lightParamKey.asString() == "posZ")
			{
				light.position.z = lightParamVal.asFloat();
			}
			if(lightParamKey.asString() == "intensityR")
			{
				light.diffuse.r =  lightParamVal.asFloat();
			}
			if(lightParamKey.asString() == "intensityG")
			{
				light.diffuse.g = lightParamVal.asFloat();
			}
			if(lightParamKey.asString() == "intensityB")
			{
				light.diffuse.b = lightParamVal.asFloat();
			}
			///Specular
			if(lightParamKey.asString() == "surfaceReflectivityR_TEMP")
			{
				light.specular.r = lightParamVal.asFloat();
			}
			if(lightParamKey.asString() == "surfaceReflectivityG_TEMP")
			{
				light.specular.g = lightParamVal.asFloat();
			}
			if(lightParamKey.asString() == "surfaceReflectivityB_TEMP")
			{
				light.specular.b = lightParamVal.asFloat();
			}
			light.ambient.r = 0.2f;
			light.ambient.g = 0.2f;
			light.ambient.b = 0.2f;
			light.linear = 0.014f;
			light.quadratic = 0.0007f;
			light.constant = 1.0f;

		}




		lights.push_back(light);
	}
	if(root["scene"]["cameras"].size() > 0)
	{
		cameras.clear();
	}
	for(Json::ValueIterator cameraIter = root["scene"]["cameras"].begin(); cameraIter != root["scene"]["cameras"].end(); ++cameraIter)
	{
		Json::Value cameraKey = cameraIter.key();
		Json::Value cameraVal = (*cameraIter);
		std::pair<std::string, Camera*> cameraPair;

		Camera* camera = new Camera();
		camera->init();
		glm::vec3 position;
		glm::vec3 lookAt;
		cameraPair.first = cameraKey.asString();
		camera->setName(cameraKey.asString());

		for(Json::ValueIterator camIt = cameraVal["camerasetup"].begin(); camIt != cameraVal["camerasetup"].end();++camIt)
		{
			Json::Value camKey = camIt.key();
			Json::Value camVal = (*camIt);


			if(camKey.asString() == "posX")
			{
				position.x = camVal.asFloat();
			}
			if(camKey.asString() == "posY")
			{
				position.y = camVal.asFloat();

			}
			if(camKey.asString() == "posZ")
			{
				position.z = camVal.asFloat();
			}
			if(camKey.asString() == "posZ")
			{
				position.z = camVal.asFloat();
			}
			if(camKey.asString() == "fov")
			{
				camera->setFieldOfView(camVal.asFloat());
			}
			if(camKey.asString() == "pitch")
			{
				camera->setPitch(camVal.asFloat());
			}
			if(camKey.asString() == "yaw")
			{
				camera->setYaw(camVal.asFloat());
			}
			if(camKey.asString() == "lookX")
			{
				lookAt.x = camVal.asFloat();
			}
			if(camKey.asString() == "lookY")
			{
				lookAt.y = camVal.asFloat();
			}
			if(camKey.asString() == "lookZ")
			{
				lookAt.z = camVal.asFloat();
			}


		}


		camera->lookAt(lookAt);
		camera->setPosition(position);
		cameraPair.second = camera;
		cameras.insert(cameraPair);
	}

	input.close();




	return true;
}

void Scene::loadDefaults()
{
	TextureLoader* t_loader = new TextureLoader();
	t_loader->LoadTexture("data\\images\\holstein1.png");
	std::pair<std::string, Texture*> texturePair;
	texturePair.first = t_loader->getName();
	texturePair.second = t_loader->getTexture();
	rManager->addToTexture(texturePair);
	std::pair<std::string, Texture*> texturePair2;
	t_loader->LoadTexture("data\\images\\default.png");
	texturePair2.first = t_loader->getName();
	texturePair2.second = t_loader->getTexture();
	rManager->addToTexture(texturePair2);

	ShaderLoader m_sloader;
	m_sloader.LoadShader("data\\shaders\\basic3.vert","data\\shaders\\basic3.Frag");
	Shader* shader = new Shader();
	shader->fragShader = m_sloader.getFrag();
	shader->vertShader = m_sloader.getVert();
	shader->programhandle = m_sloader.getProgramHandle();
	std::pair<std::string, Shader*> shaderPair;
	shaderPair.first = m_sloader.getName();
	shaderPair.second = shader;
	rManager->addToShader(shaderPair);

	m_sloader.LoadShader("data\\shaders\\TextVertexShader.vert", "data\\shaders\\TextVertexShader.Frag");
	Shader* shader2 = new Shader();
	shader2->fragShader = m_sloader.getFrag();
	shader2->vertShader = m_sloader.getVert();
	shader2->programhandle = m_sloader.getProgramHandle();
	std::pair<std::string, Shader*> shaderPair2;
	shaderPair2.first = m_sloader.getName();
	shaderPair2.second = shader2;
	rManager->addToShader(shaderPair2);
}
void Scene::InitScene(const std::string& loadSceneName)//Loads gameobjects and shaders.
{


	filename = loadSceneName;


	loadDefaults();

	if(!LoadScene(loadSceneName))
	{
		/////Default Params




		std::cout << "error loading scene\n Default data is provided.\n";
		sceneData.name = "Default";
		sceneData.currentCamera = "default";
		sceneData.currentLight = "default";
		sceneData.menu = false;
		sceneData.messageHandlers = false;
		sceneData.sceneShader = "data\\shaders\\basic3";

		//exit(EXIT_FAILURE);
	}
	if (rManager->getShaders_const().find(sceneData.sceneShader) == rManager->getShaders_const().end() ) 
	{
		programHandle = rManager->getShaders().at(sceneData.sceneShader)->programhandle;
	}
	else
	{
		programHandle = rManager->getShaders().at("data\\shaders\\basic3")->programhandle;
	}

	if(gameObjects.size() != 0 )
	{
		for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			(*it)->init(sceneData.sceneShader);
			/*for(unsigned int i =0; i < cameras.size();++i)
			{
			cameras[i]->init();
			}*/
		}
	}


}
void Scene::Update(bool keys[])//Updates the scene running in a loop
{

	/*if(keys[GLFW_KEY_LEFT_ALT])
	{
	nextCamera();
	//cameras[activeCamera]->update();
	}*/

	if(gameObjects.size() != 0 )
	{
		for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			(*it)->update(keys);

		}
	}


}
void Scene::Render()
{

	gl::UseProgram(programHandle);

	setLightParams();
	for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{

		model = glm::mat4(1);



		model = glm::translate(model,(*it)->getTransformComp()->getTranslate());
		model = glm::rotate(model,glm::radians((*it)->getTransformComp()->getRotate().x),glm::vec3(1.0f,0.0f,0.0f));
		model = glm::rotate(model,glm::radians((*it)->getTransformComp()->getRotate().y),glm::vec3(0.0f,1.0f,0.0f));
		model = glm::rotate(model,glm::radians((*it)->getTransformComp()->getRotate().z),glm::vec3(0.0f,0.0f,1.0f));
		model = glm::scale(model,(*it)->getTransformComp()->getScale());


		setUpMatricies();
		(*it)->render();

	}

}
void Scene::resize(int w, int h)
{
	gl::Viewport(0,0,w,h);
	if(cameras.size() > 0)
	{
		cameras.at(activeCamera)->setAspectRatio((float)w/h);
	}
}

void Scene::nextCamera()
{
	for(auto it = cameras.begin(); it != cameras.end(); ++it)
	{
		if(it->first == activeCamera)
		{

			if(it == --cameras.end())//
			{
				it = cameras.begin();
			}
			else
			{
				it++;
			}
			activeCamera = it->first;
			return;
		}
	}
}


void Scene::setLightParams()
{


	GLuint loc5 = gl::GetUniformLocation(programHandle, "numLights");
	gl::Uniform1i(loc5, lights.size());
	for(unsigned int i =0; i< lights.size();++i)
	{
		std::ostringstream oss;
		oss << "lights[" << i << "].diffuse";
		std::string var = oss.str();
		GLuint loc = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform3f(loc,lights.at(i).diffuse.r,lights.at(i).diffuse.g,lights.at(i).diffuse.b);
		oss.clear();
		oss.str("");
		oss << "lights[" << i << "].ambient";
		var = oss.str();
		GLuint loc1 = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform3f(loc1,lights.at(i).ambient.r,lights.at(i).ambient.g,lights.at(i).ambient.b);
		oss.clear();
		oss.str("");
		oss << "lights[" << i << "].position";
		var = oss.str();
		GLuint loc2 = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform3f( loc2,lights.at(i).position.x,lights.at(i).position.y,lights.at(i).position.z);
		oss.clear();
		oss.str("");
		oss << "lights[" << i << "].linear";
		var = oss.str();
		GLuint loc3 = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform1f( loc3,lights.at(i).linear);
		oss.clear();
		oss.str("");
		oss << "lights[" << i << "].constant";
		var = oss.str();
		GLuint loc4 = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform1f( loc4,lights.at(i).constant);
		oss.clear();
		oss.str("");
		oss << "lights[" << i << "].quadratic";
		var = oss.str();
		GLuint loc5 = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform1f( loc5,lights.at(i).quadratic);
		oss.clear();
		oss.str("");
		oss << "lights[" << i << "].specular";
		var = oss.str();
		GLuint loc6 = gl::GetUniformLocation(programHandle, var.c_str());
		gl::Uniform3f(loc6,lights.at(i).specular.r,lights.at(i).specular.g,lights.at(i).specular.b);


	}
	GLuint loc= gl::GetUniformLocation(programHandle, "Kd");
	gl::Uniform3f(loc,1.0f, 1.0f, 1.0f);
	GLuint loc1= gl::GetUniformLocation(programHandle, "Ks");
	gl::Uniform3f(loc1,1.0f,1.0f,1.0f);
	GLuint loc2= gl::GetUniformLocation(programHandle, "shininess");
	gl::Uniform1f(loc2,32.0f);
}
void Scene::setUpMatricies()
{

	GLuint loc1 = gl::GetUniformLocation(programHandle,"NormalMatrix");

	glm::mat3 normMat = glm::transpose(glm::inverse(glm::mat3(model)));
	gl::UniformMatrix3fv(loc1, 1, FALSE, &normMat[0][0] );

	GLuint viewMatrixID = 
		gl::GetUniformLocation(programHandle, "V");//programhandle in first parameter
	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, &cameras.at(activeCamera)->view()[0][0]);

	GLuint projectionMatrixID = 
		gl::GetUniformLocation(programHandle, "P");//programhandle in first parameter

	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, &cameras.at(activeCamera)->projection()[0][0]);

	GLuint modelMatrixID = gl::GetUniformLocation(programHandle, "M");
	gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &model[0][0]);

}
void Scene::deleteShader()
{
	gl::DeleteProgram(programHandle);
	/*for(int i=0; i< rManager->getShaders().size();++i)
	{
	gl::DeleteShader(rManager->getShaders().at(i)->fragShader);
	gl::DeleteShader(rManager->getShaders().at(i)->vertShader);
	}*/

}


const Json::Value Scene::createJson() 
{
	Json::Value root;
	root["scene"]["name"] = sceneData.name;
	root["scene"]["sceneshader"] = sceneData.sceneShader;
	root["scene"]["currentcamera"] = sceneData.currentCamera;
	root["scene"]["currentlight"] = sceneData.currentLight;
	root["scene"]["messagehandlers"] = sceneData.messageHandlers;
	root["scene"]["menu"] = sceneData.menu;
	for(unsigned int i =0 ; i < gameObjects.size(); ++i)
	{
		root["scene"]["activeentities"][i] = gameObjects.at(i)->getName();
		root["scene"]["entities"][gameObjects.at(i)->getName()]["type"] = gameObjects.at(i)->getEntityType();
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["posX"] = gameObjects.at(i)->getTransformComp()->getTranslate().x;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["posY"] = gameObjects.at(i)->getTransformComp()->getTranslate().y;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["posZ"] = gameObjects.at(i)->getTransformComp()->getTranslate().z;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["scaleX"] =  gameObjects.at(i)->getTransformComp()->getScale().x;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["scaleY"] =  gameObjects.at(i)->getTransformComp()->getScale().y;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["scaleZ"] =  gameObjects.at(i)->getTransformComp()->getScale().z;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["rotateX"] =  gameObjects.at(i)->getTransformComp()->getRotate().x;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["rotateY"] =  gameObjects.at(i)->getTransformComp()->getRotate().y;
		root["scene"]["entities"][gameObjects.at(i)->getName()]["transform"]["rotateZ"] =  gameObjects.at(i)->getTransformComp()->getRotate().z;

		for(unsigned int j=0; j < gameObjects.at(i)->getComponentIDs().size(); ++j)
		{
			root["scene"]["entities"][gameObjects.at(i)->getName()]["components"][gameObjects.at(i)->getComponentIDs().at(j)]["id"] = gameObjects.at(i)->getComponentIDs().at(j);
			root["scene"]["entities"][gameObjects.at(i)->getName()]["components"][gameObjects.at(i)->getComponentIDs().at(j)]["type"] =  gameObjects.at(i)->getComponentTypes().at(j);
			root["scene"]["entities"][gameObjects.at(i)->getName()]["components"][gameObjects.at(i)->getComponentIDs().at(j)]["OBJModel"] = gameObjects.at(i)->getComponentModelFiles().at(j);
			root["scene"]["entities"][gameObjects.at(i)->getName()]["components"][gameObjects.at(i)->getComponentIDs().at(j)]["TextureFile"] = gameObjects.at(i)->getComponentTextureFiles().at(j);

		}

	}
	for(auto it = cameras.begin(); it != cameras.end(); ++it)
	{

		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["posX"] = it->second->position().x;
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["posY"] = it->second->position().y;
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["posZ"] = it->second->position().z;
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["lookX"] = it->second->getTarget().x;
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["lookY"] = it->second->getTarget().y;
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["lookZ"] = it->second->getTarget().z;
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["fov"] = it->second->fieldOfView();
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["pitch"] = it->second->getPitch();
		root["scene"]["cameras"][it->second->getName()]["camerasetup"]["yaw"] = it->second->getYaw();
	}
	for(unsigned int i=0; i < lights.size(); ++i)
	{
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["posX"] = lights.at(i).position.x;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["posY"] = lights.at(i).position.y;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["posZ"] = lights.at(i).position.z;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["intensityR"] = lights.at(i).diffuse.r;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["intensityG"] = lights.at(i).diffuse.g;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["intensityB"] = lights.at(i).diffuse.b;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["surfaceReflectivityR_TEMP"] = lights.at(i).specular.r;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["surfaceReflectivityG_TEMP"] = lights.at(i).specular.g;
		root["scene"]["lights"][lights.at(i).name]["lightparams"]["surfaceReflectivityB_TEMP"] = lights.at(i).specular.b;
	}

	return root;
}


